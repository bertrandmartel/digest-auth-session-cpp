##################################################################################
# The MIT License (MIT)
#
# Copyright (c) 2015 Bertrand Martel
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
###################################################################################

#!/bin/bash
#title         : digest_test.sh
#author		   : Bertrand Martel
#date          : 02/09/2015
#description   : test digest authentication / digest session / cookie session
############################################################################

# FOR DIGEST AUTHENTICATION - COOKIE SESSION
# 1) Test authentication success
# 2) Test cookie hsid is enabled
# 3) Test cookie hsid is not valid 
# 4) Test opaque invalid
# 5) Test digest-uri invalid
# 6) Test nonce count invalid
# 7) Test nonce count > 1
# 8) Test cookie lifetime [ TESTED MANUALLY ]
# 9) Test nonce lifetime  [ TESTED MANUALLY ]

# FOR DIGEST AUTHENTICATION - DIGEST SESSION
# 1) Test authentication success
# 2) Test opaque invalid
# 3) Test digest-uri invalid
# 4) Test nonce count invalid
# 5) Test nonce count > 1
# 6) Test 5 consecutive session key with different nonce count
# 7) Test cookie lifetime [ TESTED MANUALLY ]
# 8) Test nonce lifetime  [ TESTED MANUALLY ]

verbose=false

if [ "$1" == "-v" ]; then
	verbose=true
fi

protocol="http"
uri="/login"
username="test"
password="test"
method="GET"
cnonce="12345"

test_success=0
test_count=7

function test_success {
	test_success=$((test_success + 1))
	echo -e "\x1B[01;32m[ ${test_success} / ${test_count} ] [SUCCESS] $1 \x1B[0m"
}

function test_error {
	echo -e "\x1B[31m$1 [FAILURE] \x1B[0m"
}

function launch_request {

	request_headers=`curl -sk -D -  ${protocol}://127.0.0.1:4343/login | grep  'WWW-Authenticate\|Set-Cookie: HSID'`

	request_no_header_authenticate=`echo ${request_headers} | grep WWW-Authenticate`
	request_no_header_cookie=`echo ${request_headers} | grep Cookie`

	if [[ -z $request_no_header_authenticate ]]; then
		echo "Expected response with WWW-authenticate header. Exiting ...."
		exit 0;
	elif [[ -z $request_no_header_cookie ]]; then
		echo "Expected response with Cookie header. Exiting ...."
		exit 0;
	fi

	realm=`echo $request_no_header_authenticate | grep -Eo 'realm="[a-zA-Z0-9=\+\/]+"' | sed -e 's/realm="//g' | sed -e 's/"//g'`
	nonce=`echo $request_no_header_authenticate | grep -Eo 'nonce="[a-zA-Z0-9=\+\/]+"' | sed -e 's/nonce="//g' | sed -e 's/"//g'`
	opaque=`echo $request_no_header_authenticate | grep -Eo 'opaque="[a-zA-Z0-9=\+\/]+"' | sed -e 's/opaque="//g' | sed -e 's/"//g'`

	hsid=`echo $request_no_header_cookie | grep -oE 'HSID=[a-zA-Z0-9\/=\+]+' | sed -e 's/HSID=//g'`

	current_date=`date +%T`

	if $verbose ; then
		echo "#####################################"
		echo "[$current_date] Received response from server with : "
		echo "[$current_date] realm  : $realm"
		echo "[$current_date] nonce  : $nonce"
		echo "[$current_date] opaque : $opaque"
		echo "[$current_date] hsid   : $hsid"
		echo "#####################################"
	fi

	first_part=`echo -n "${username}:${realm}:${password}" | $1 | grep -oE '\w+'`
	last_part=`echo -n "${method}:${uri}" | $1 | grep -oE '\w+'`
}

function launch_cookie_digest_auth_request {

	######################################################################################################################################
	########################### TEST 1 : Test authentication success #####################################################################
	######################################################################################################################################
	test_name="Test authentication success"

	launch_request "$1"

	response_digest=`echo -n "${first_part}:${nonce}:00000001:${cnonce}:auth-int:${last_part}" | $1 | grep -oE '\w+'`

	authentication_request=`curl -sk -D - --header "Authorization: algorithm="MD5",opaque="${opaque}",digest-uri="${uri}",username="${username}",qop="auth-int",nc=00000001,response=${response_digest},cnonce=${cnonce}" --header "Cookie: HSID=${hsid}; Domain=127.0.0.1; Path=/; HttpOnly" ${protocol}://127.0.0.1:4343${uri} | grep "HTTP/1.1 200"`

	if [[ ! -z $authentication_request ]]; then
		test_success "${test_name}"
	else
		test_error "${test_name}"
		exit 0;
	fi

	######################################################################################################################################
	########################### TEST 2 : Test cookie hsid is valid## #####################################################################
	######################################################################################################################################
	test_name="Test cookie hsid is valid"
	request_headers=`curl -sk -D - --header "Cookie: HSID=${hsid}; Domain=127.0.0.1; Path=/; HttpOnly" ${protocol}://127.0.0.1:4343/login | grep "HTTP/1.1 200"`

	if [[ ! -z $request_headers ]]; then
		test_success "${test_name}"
	else
		test_error "${test_name}"
		exit 0;
	fi

	######################################################################################################################################
	########################### TEST 3 : Test cookie hsid is not valid  ##################################################################
	######################################################################################################################################
	test_name="Test cookie hsid is not valid"
	request_headers=`curl -sk -D - --header "Cookie: HSID=${hsid}FAKE; Domain=127.0.0.1; Path=/; HttpOnly" ${protocol}://127.0.0.1:4343/login | grep "HTTP/1.1 401"`

	if [[ ! -z $request_headers ]]; then
		test_success "${test_name}"
	else
		test_error "${test_name}"
		exit 0;
	fi

	######################################################################################################################################
	########################### TEST 4 : Test opaque invalid #############################################################################
	######################################################################################################################################
	test_name="Test opaque invalid"

	launch_request "$1"

	response_digest=`echo -n "${first_part}:${nonce}:00000001:${cnonce}:auth-int:${last_part}" | $1 | grep -oE '\w+'`

	authentication_request=`curl -sk -D - --header "Authorization: algorithm="MD5",opaque="${opaque}FAKE",digest-uri="${uri}",username="${username}",qop="auth-int",nc=00000001,response=${response_digest},cnonce=${cnonce}" --header "Cookie: HSID=${hsid}; Domain=127.0.0.1; Path=/; HttpOnly" ${protocol}://127.0.0.1:4343${uri} | grep "HTTP/1.1 401"`

	if [[ ! -z $authentication_request ]]; then
		test_success "${test_name}"
	else
		test_error "${test_name}"
		exit 0;
	fi

	######################################################################################################################################
	########################### TEST 5 : Test digest-uri invalid ########################################################################
	######################################################################################################################################
	test_name="Test digest-uri invalid"

	launch_request "$1"

	response_digest=`echo -n "${first_part}:${nonce}:00000001:${cnonce}:auth-int:${last_part}" | $1 | grep -oE '\w+'`

	authentication_request=`curl -sk -D - --header "Authorization: algorithm="MD5",opaque="${opaque}",digest-uri="${uri}FAKE",username="${username}",qop="auth-int",nc=00000001,response=${response_digest},cnonce=${cnonce}" --header "Cookie: HSID=${hsid}; Domain=127.0.0.1; Path=/; HttpOnly" ${protocol}://127.0.0.1:4343${uri} | grep "HTTP/1.1 401"`

	if [[ ! -z $authentication_request ]]; then
		test_success "${test_name}"
	else
		test_error "${test_name}"
		exit 0;
	fi

	######################################################################################################################################
	########################### TEST 6 : Test nonce count invalid ########################################################################
	######################################################################################################################################
	test_name="Test nonce count invalid"

	launch_request "$1"

	response_digest=`echo -n "${first_part}:${nonce}:00000001:${cnonce}:auth-int:${last_part}" | $1 | grep -oE '\w+'`

	authentication_request=`curl -sk -D - --header "Authorization: algorithm="MD5",opaque="${opaque}",digest-uri="${uri}",username="${username}",qop="auth-int",nc=00000009,response=${response_digest},cnonce=${cnonce}" --header "Cookie: HSID=${hsid}; Domain=127.0.0.1; Path=/; HttpOnly" ${protocol}://127.0.0.1:4343${uri} | grep "HTTP/1.1 401"`

	if [[ ! -z $authentication_request ]]; then
		test_success "${test_name}"
	else
		test_error "${test_name}"
		exit 0;
	fi

	######################################################################################################################################
	########################### TEST 7 : Test nonce count > 1 ############################################################################
	######################################################################################################################################
	test_name="Test nonce count > 1"

	response_digest=`echo -n "${first_part}:${nonce}:00000002:${cnonce}:auth-int:${last_part}" | $1 | grep -oE '\w+'`

	authentication_request=`curl -sk -D - --header "Authorization: algorithm="MD5",opaque="${opaque}",digest-uri="${uri}",username="${username}",qop="auth-int",nc=00000002,response=${response_digest},cnonce=${cnonce}" --header "Cookie: HSID=${hsid}; Domain=127.0.0.1; Path=/; HttpOnly" ${protocol}://127.0.0.1:4343${uri} | grep "HTTP/1.1 200"`

	if [[ ! -z $authentication_request ]]; then
		test_success "${test_name}"
	else
		test_error "${test_name}"
		exit 0;
	fi
}

function launch_request_digest_session {

	request_headers=`curl -sk -D -  ${protocol}://127.0.0.1:4343/login | grep  'WWW-Authenticate'`

	request_no_header_authenticate=`echo ${request_headers} | grep WWW-Authenticate`

	if [[ -z $request_no_header_authenticate ]]; then
		echo "Expected response with WWW-authenticate header. Exiting ...."
		exit 0;
	fi

	realm=`echo $request_no_header_authenticate | grep -Eo 'realm="[a-zA-Z0-9=\+\/]+"' | sed -e 's/realm="//g' | sed -e 's/"//g'`
	nonce=`echo $request_no_header_authenticate | grep -Eo 'nonce="[a-zA-Z0-9=\+\/]+"' | sed -e 's/nonce="//g' | sed -e 's/"//g'`
	opaque=`echo $request_no_header_authenticate | grep -Eo 'opaque="[a-zA-Z0-9=\+\/]+"' | sed -e 's/opaque="//g' | sed -e 's/"//g'`

	current_date=`date +%T`

	if $verbose ; then
		echo "#####################################"
		echo "[$current_date] Received response from server with : "
		echo "[$current_date] realm  : $realm"
		echo "[$current_date] nonce  : $nonce"
		echo "[$current_date] opaque : $opaque"
		echo "#####################################"
	fi

	first_part=`echo -n "${username}:${realm}:${password}" | $1 | grep -oE '\w+'`
	first_part="$first_part:${nonce}:${cnonce}"

	last_part=`echo -n "${method}:${uri}" | $1 | grep -oE '\w+'`
}

function launch_digest_session_digest_auth_request {

	######################################################################################################################################
	########################### TEST 1 : Test authentication success #####################################################################
	######################################################################################################################################
	test_name="Test authentication success"

	launch_request_digest_session "$1"

	response_digest=`echo -n "${first_part}:${nonce}:00000001:${cnonce}:auth-int:${last_part}" | $1 | grep -oE '\w+'`

	authentication_request=`curl -sk -D - --header "Authorization: algorithm="MD5",opaque="${opaque}",digest-uri="${uri}",username="${username}",qop="auth-int",nc=00000001,response=${response_digest},cnonce=${cnonce}" ${protocol}://127.0.0.1:4343${uri} | grep "HTTP/1.1 200"`

	if [[ ! -z $authentication_request ]]; then
		test_success "${test_name}"
	else
		test_error "${test_name}"
		exit 0;
	fi

	######################################################################################################################################
	########################### TEST 2 : Test opaque invalid #############################################################################
	######################################################################################################################################
	test_name="Test opaque invalid"

	launch_request_digest_session "$1"

	response_digest=`echo -n "${first_part}:${nonce}:00000001:${cnonce}:auth-int:${last_part}" | $1 | grep -oE '\w+'`

	authentication_request=`curl -sk -D - --header "Authorization: algorithm="MD5",opaque="${opaque}FAKE",digest-uri="${uri}",username="${username}",qop="auth-int",nc=00000001,response=${response_digest},cnonce=${cnonce}" ${protocol}://127.0.0.1:4343${uri} | grep "HTTP/1.1 401"`

	if [[ ! -z $authentication_request ]]; then
		test_success "${test_name}"
	else
		test_error "${test_name}"
		exit 0;
	fi

	######################################################################################################################################
	########################### TEST 3 : Test digest-uri invalid ########################################################################
	######################################################################################################################################
	test_name="Test digest-uri invalid"

	launch_request_digest_session "$1"

	response_digest=`echo -n "${first_part}:${nonce}:00000001:${cnonce}:auth-int:${last_part}" | $1 | grep -oE '\w+'`

	authentication_request=`curl -sk -D - --header "Authorization: algorithm="MD5",opaque="${opaque}",digest-uri="${uri}FAKE",username="${username}",qop="auth-int",nc=00000001,response=${response_digest},cnonce=${cnonce}" ${protocol}://127.0.0.1:4343${uri} | grep "HTTP/1.1 401"`

	if [[ ! -z $authentication_request ]]; then
		test_success "${test_name}"
	else
		test_error "${test_name}"
		exit 0;
	fi

	######################################################################################################################################
	########################### TEST 4 : Test nonce count invalid ########################################################################
	######################################################################################################################################
	test_name="Test nonce count invalid"

	launch_request_digest_session "$1"

	response_digest=`echo -n "${first_part}:${nonce}:00000001:${cnonce}:auth-int:${last_part}" | $1 | grep -oE '\w+'`

	authentication_request=`curl -sk -D - --header "Authorization: algorithm="MD5",opaque="${opaque}",digest-uri="${uri}",username="${username}",qop="auth-int",nc=00000009,response=${response_digest},cnonce=${cnonce}" ${protocol}://127.0.0.1:4343${uri} | grep "HTTP/1.1 401"`

	if [[ ! -z $authentication_request ]]; then
		test_success "${test_name}"
	else
		test_error "${test_name}"
		exit 0;
	fi

	######################################################################################################################################
	########################### TEST 5 : Test nonce count > 1 ############################################################################
	######################################################################################################################################
	test_name="Test nonce count > 1"

	response_digest=`echo -n "${first_part}:${nonce}:00000002:${cnonce}:auth-int:${last_part}" | $1 | grep -oE '\w+'`

	authentication_request=`curl -sk -D - --header "Authorization: algorithm="MD5",opaque="${opaque}",digest-uri="${uri}",username="${username}",qop="auth-int",nc=00000002,response=${response_digest},cnonce=${cnonce}" ${protocol}://127.0.0.1:4343${uri} | grep "HTTP/1.1 200"`

	if [[ ! -z $authentication_request ]]; then
		test_success "${test_name}"
	else
		test_error "${test_name}"
		exit 0;
	fi

	######################################################################################################################################
	########################### TEST 6 : Test 5 consecutive session key with different nonce count #######################################
	######################################################################################################################################
	test_name="Test 5 consecutive session key with different nonce count"

	launch_request_digest_session "$1"

	nonce_count=1

	for i in {1..5}
	do
		response_digest=`echo -n "${first_part}:${nonce}:0000000${nonce_count}:${cnonce}:auth-int:${last_part}" | $1 | grep -oE '\w+'`

		authentication_request=`curl -sk -D - --header "Authorization: algorithm="MD5",opaque="${opaque}",digest-uri="${uri}",username="${username}",qop="auth-int",nc=0000000${nonce_count},response=${response_digest},cnonce=${cnonce}" ${protocol}://127.0.0.1:4343${uri} | grep "HTTP/1.1 200"`

		if [[ -z $authentication_request ]]; then
			test_error "${test_name}"
			exit 0;
		fi
		nonce_count=$((nonce_count+1))
	done

	test_success "${test_name}"
}

algo="MD5"
session_type="COOKIE"

if [ "$1" == "MD5" ] || [ "$1" == "SHA1" ]; then
	algo="$1"
fi

if [ "$2" == "DIGEST" ] || [ "$2" == "COOKIE" ]; then
	session_type="$2"
fi

echo "#######################################################"

if [ "$algo" == "MD5" ] && [ "$session_type" == "COOKIE" ]; then
	echo "DIGEST AUTHENTICATION WITH COOKIE SESSION - ALGO => MD5"
	launch_cookie_digest_auth_request "md5sum"
elif [ "$algo" == "SHA1" ] && [ "$session_type" == "COOKIE" ]; then
	echo "DIGEST AUTHENTICATION WITH COOKIE SESSION - ALGO => SHA1"
	launch_cookie_digest_auth_request "sha1sum"
elif [ "$algo" == "MD5" ] && [ "$session_type" == "DIGEST" ]; then
	test_count=6
	launch_digest_session_digest_auth_request "md5sum"
elif [ "$algo" == "SHA1" ] && [ "$session_type" == "DIGEST" ]; then
	test_count=6
	launch_digest_session_digest_auth_request "sha1sum"
else
	echo "Usage           : test_authentication_session.sh <algo> <session_type>"
	echo "<algo>          : MD5    | SHA1 "
	echo "<session_type>  : COOKIE | DIGEST "
fi

echo "#######################################################"