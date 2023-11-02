#pragma once

#include "AES.h"

#include "Poco/JWT/Signer.h"
#include "Poco/JWT/Token.h"

#include <string>

class JWT {
public:
    JWT(const std::string& publicKeyFile, const std::string& privateKeyFile, std::string passphrase);

    [[nodiscard]] std::string generateAccessToken(const std::string& data) const;
    [[nodiscard]] std::string generateRefreshToken(const std::string& data) const;

    [[nodiscard]] bool verifyToken(const std::string& jwt, std::string& data) const;

private:
    [[nodiscard]] Poco::JWT::Token getToken(const std::string& data, int expiration) const;
    std::string signToken(Poco::JWT::Token& unsignedToken) const;

    static const int ACCESS_TOKEN_EXPIRATION = 15 * 60;
    static const int REFRESH_TOKEN_EXPIRATION = 30 * 24 * 60 * 60;

    Poco::JWT::Signer _signer;
    const std::string _passphrase;
};