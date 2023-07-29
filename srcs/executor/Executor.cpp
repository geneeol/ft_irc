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

bool Executor::Visit(NickRequest *nickRequest) const
{
    Client *client = nickRequest->GetClient();

    client->SetNickName(nickRequest->GetNickName());
    client->SetNickNameEntered();

    if (client->HasRegistered())
    {
        std::stringstream responseMessage;
        responseMessage << client->GetClientInfo() << "NICK :" << nickRequest->GetNickName();

        Channel *channel = client->GetChannel();
        if (channel)
            channel->BroadcastMessage(responseMessage.str());
        else
            client->InsertResponse(responseMessage.str());
    }

    if (!client->HasRegistered() && client->HasEnteredUserInfo() && client->HasEnteredPassword())
    {
        // TODO Welcome message 뿌려주기
        EnvManager *envManager = EnvManager::GetInstance();

        std::stringstream welcomeMessage;
        welcomeMessage << ":" << envManager->GetServerName() << " 001 " << client->GetNickName()
                       << " :Welcome to the PingPong IRC Network " << client->GetClientInfo();

        client->InsertResponse(welcomeMessage.str());
        client->SetRegistered();
    }

    return true;
}

bool Executor::Visit(PartRequest *partRequest) const
{
    Client *client = partRequest->GetClient();

    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    Channel *channel = channelRepository->FindByName(partRequest->GetChannelName());

    std::stringstream responseMessage;
    responseMessage << client->GetClientInfo() << " PART " << partRequest->GetChannelName();

    if (!partRequest->GetReason().empty())
        responseMessage << " :" << partRequest->GetReason();

    channel->BroadcastMessage(responseMessage.str());

    // Operator 이면 지워주는 것은 내부에서 알아서 해줌
    channel->RemoveClient(client->GetNickName());
    // TODO Shared Ptr 이면 delete?
    client->SetChannel(NULL);

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
    EnvManager *envManager = EnvManager::GetInstance();
    std::string serverName = envManager->GetServerName();

    std::stringstream responseMessage;
    responseMessage << ":" << serverName << " PONG " << serverName << " :" << pingRequest->GetToken();

    Client *client = pingRequest->GetClient();
    client->InsertResponse(responseMessage.str());

    return true;
}

bool Executor::Visit(PrivmsgRequest *privmsgRequest) const
{
    ChannelRepository *channelRepository = ChannelRepository::GetInstance();
    ClientRepository *clientRepository = ClientRepository::GetInstance();

    Client *client = privmsgRequest->GetClient();
    std::stringstream responseMessage;

    std::vector<std::string> targets = privmsgRequest->GetTargets();
    std::vector<std::string>::iterator iter;

    for (iter = targets.begin(); iter != targets.end(); iter++)
    {
        responseMessage << client->GetClientInfo() << " PRIVMSG " << *iter << ":" << privmsgRequest->GetMessage();

        if (iter->front() == '#')
        {
            Channel *targetChannel = channelRepository->FindByName(*iter);
            targetChannel->BroadcastMessage(responseMessage.str());
        }
        else
        {
            Client *targetClient = clientRepository->FindByNickname(*iter);
            targetClient->InsertResponse(responseMessage.str());
        }
    }

    return true;
}

bool Executor::Visit(QuitRequest *quitRequest) const
{
    Client *client = quitRequest->GetClient();

    std::stringstream responseMessage;

    // TODO hostname 수정
    responseMessage << "ERROR :Closing link: (" << client->GetUserName() << "@" << client->GetHostName() << ")";

    if (quitRequest->GetReason().empty())
        responseMessage << " [Client exited]";
    else
        responseMessage << " [Quit: " << quitRequest->GetReason() << "]";

    client->InsertResponse(responseMessage.str());

    Channel *channel = client->GetChannel();

    if (channel)
    {
        std::stringstream responseMessage;

        responseMessage << client->GetClientInfo() << " QUIT :";

        if (quitRequest->GetReason().empty())
            responseMessage << "Client exited";
        else
            responseMessage << "Quit : " << quitRequest->GetReason();

        channel->BroadcastMessage(responseMessage.str());
        channel->RemoveClient(client->GetNickName());
    }

    ClientRepository *clientRepository = ClientRepository::GetInstance();

    clientRepository->RemoveClient(client->GetSocket(), client->GetNickName());

    // TODO 소켓 닫아주기 - Client 소멸자에서 하는 방식으로 구성?

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
    topicChangedMsg << topicRequest->GetClient()->GetClientInfo() << "TOPIC " << topicRequest->GetChannelName() << " :"
                    << newTopic;

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

    if (client->HasEnteredNickName() && client->HasEnteredPassword())
    {
        // TODO Welcome message 뿌려주기
        EnvManager *envManager = EnvManager::GetInstance();

        std::stringstream welcomeMessage;
        welcomeMessage << ":" << envManager->GetServerName() << " 001 " << client->GetNickName()
                       << " :Welcome to the PingPong IRC Network " << client->GetClientInfo();

        client->InsertResponse(welcomeMessage.str());
        client->SetRegistered();
    }

    return true;
}