#ifndef CHANNEL_REPOSITORY_H
#define CHANNEL_REPOSITORY_H

#include <iostream>
#include <map>
#include "../singleton_template/SingletonTemplate.hpp"
#include "../channel/Channel.hpp"

class ChannelRepository : TSingleton<ChannelRepository> {

public:
    //JOIN은 있으면 만들지만 나머지는 예외처리 ! 채널 만드는 거 따로
    Channel *CreateChannel(std::string &name);
    Channel *FindByName(std::string &name) const;

private:
    //TODO shared_ptr
    std::map<std::string, Channel *> mChannels;

};

#endif