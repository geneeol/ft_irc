#include "Executor.hpp"

bool Executor::Visit(CapRequest *capRequest) const
{

    return true;
}

bool Executor::Visit(InviteRequest *inviteRequest) const
{

    return true;
}

bool Executor::Visit(JoinRequest *joinRequest) const
{

    return true;
}

bool Executor::Visit(KickRequest *kickRequest) const
{

    return true;
}

bool Executor::Visit(ModeRequest *modeRequest) const
{

    return true;
}

// TODO 클라이언트가 속한 채널 가져오기
bool Executor::Visit(NickRequest *nickRequest) const
{
    Client *client = nickRequest->GetClient();

    client->SetNickName(nickRequest->GetNickName());
    client->SetNickNameEntered();

    std::stringstream responseMessage;

    if (client->Registered())
    {
        responseMessage << client->GetClientInfo() << "NICK :" << nickRequest->GetNickName();

        if (클라이언트가 어떠한 채널에 속해 있으면)
        {
            Channel *channel; // 클라이언트가 속한 채널
            channel->BroadcastMessage(responseMessage);
        }
        else
            client->InsertResponse(responseMessage);
    }

    if (!client->Registered() && client->EnteredUserInfo() && client->EnteredPassword())
    {
        // TODO Welcome message 뿌려주기
        client->SetRegistered();
    }

    return true;
}

bool Executor::Visit(PartRequest *partRequest) const
{

    return true;
}

bool Executor::Visit(PassRequest *passRequest) const
{
    Client *client = passRequest->GetClient();

    client->SetPasswordEntered();

    return true;
}

bool Executor::Visit(PingRequest *pingRequest) const
{

    return true;
}

bool Executor::Visit(PrivmsgRequest *privmsgRequest) const
{

    return true;
}

bool Executor::Visit(QuitRequest *quitRequest) const
{

    return true;
}

bool Executor::Visit(TopicRequest *topicRequest) const
{
    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    Channel *channel = channelRepository->FindByName(topicRequest->GetChannelName());

    std::string newTopic = topicRequest->GetTopic();

    if (newTopic == channel->GetTopic())
        return true;

    channel->SetTopic(newTopic);

    std::stringstream topicChangedMsg;
    topicChangedMsg << topicRequest->GetClient()->GetClientInfo() << "TOPIC #"
                    << topicRequest->GetChannelName() << " :" << newTopic;

    channel->BroadcastMessage(topicChangedMsg.str());
    return true;
}

bool Executor::Visit(UserRequest *userRequest) const
{
    Client *client = userRequest->GetClient();

    client->SetUserName(userRequest->GetUserName());
    client->SetHostName(userRequest->GetHostName());
    client->SetServerName(userRequest->GetServerName());
    client->SetRealName(userRequest->GetRealName());
    client->SetUserInfoEntered();

    if (client->EnteredNickName() && client->EnteredPassword())
    {
        // TODO Welcome message 뿌려주기
        client->SetRegistered();
    }

    return true;
}
