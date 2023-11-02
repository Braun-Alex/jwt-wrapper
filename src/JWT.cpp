#include "../include/JWT.h"

JWT::JWT(const std::string& publicKeyFile, const std::string& privateKeyFile, std::string passphrase): _signer(
        new Poco::Crypto::ECKey(publicKeyFile, privateKeyFile)), _passphrase(std::move(passphrase)) {
    _signer.addAlgorithm("ES256");
}

Poco::JWT::Token JWT::getToken(const std::string& data, int expiration) const {
    Poco::JWT::Token token;
    token.setType("JWT");
    token.setSubject(AES::encrypt(data, _passphrase));
    Poco::Timestamp currentTimestamp = Poco::Timestamp();
    token.setIssuedAt(currentTimestamp);
    token.setExpiration(currentTimestamp + expiration * Poco::Timestamp::resolution());
    return token;
}

std::string JWT::generateAccessToken(const std::string& data) const {
    Poco::JWT::Token unsignedAccessToken = getToken(data, ACCESS_TOKEN_EXPIRATION);
    return signToken(unsignedAccessToken);
}

std::string JWT::generateRefreshToken(const std::string& data) const {
    Poco::JWT::Token unsignedRefreshToken = getToken(data, REFRESH_TOKEN_EXPIRATION);
    return signToken(unsignedRefreshToken);
}

std::string JWT::signToken(Poco::JWT::Token& unsignedToken) const {
    return _signer.sign(unsignedToken, Poco::JWT::Signer::ALGO_ES256);
}

bool JWT::verifyToken(const std::string& jwt, std::string& data) const {
    Poco::JWT::Token token;
    bool isValid = _signer.tryVerify(jwt, token);
    if (!isValid && !token.payload().has("exp")) {
        return false;
    }
    Poco::Timestamp expiration = token.getExpiration();
    if (Poco::Timestamp() >= expiration) {
        return false;
    }
    data = AES::decrypt(token.getSubject(), _passphrase);
    return true;
}