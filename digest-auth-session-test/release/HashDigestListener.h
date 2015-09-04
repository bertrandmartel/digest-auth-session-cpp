#ifndef HASHDIGESTLISTENER_H
#define HASHDIGESTLISTENER_H

#include "string"
#include "IDigestListener.h"
#include "digeststruct.h"

class HashDigestListener : public IDigestListener {

    public:

    /**
     * @brief ClientSocketObj::ClientSocketObj
     *      Build one client
     */
    HashDigestListener();

    ~HashDigestListener();

    /**
     * @brief get_hash_for_username
     *      retrieve hash in database or in your configuration for specified username and realm
     * @return
     */
    std::string get_hash_for_username(std::string username,std::string realm,digest_algorithm algo);

};

#endif // HASHDIGESTLISTENER_H
