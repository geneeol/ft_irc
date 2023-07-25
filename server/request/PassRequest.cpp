#include "PassRequest.hpp"

PassRequest::PassRequest(handle_t socket) : Request(socket) {}

void UserRequest::ValidateRequest() const {

}

Command *UserRequest::BuildCommand() const {
    UserCommand *userCommand = new UserCommand(this);

    return userCommand;
}

void PassRequest::SetPassword(const std::string &password) {
    mPassword = password;
}

const std::string &PassRequest::GetPassword() const {
    return mPassword;
}