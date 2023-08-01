#include "Channel.hpp"

Channel::Channel(const std::string &name)
    : mName(name), mTopic(std::string()), mKey(std::string()), mClientLimit(0), mModeFlags(EMPTY_FLAGS)
{
    LOG_TRACE("Channel constructor called | " << *this);
}

void Channel::SetClientLimit(int limit)
{
    mClientLimit = limit;
}

int Channel::GetClientCount() const
{
    return mClients.size();
}

int Channel::GetClientLimit() const
{
    return mClientLimit;
}

// TODO: dahkang 현재 mClients 벡터만 순회중
void Channel::BroadcastMessage(const std::string &message)
{
    std::vector<Client *>::iterator iter = mClients.begin();

    while (iter != mClients.end())
    {
        (*iter)->addResponseToBuf(message);
        iter++;
    }
}

void Channel::BroadcastMessageExcludingRequestor(const std::string &message, const std::string &requestorNickName)
{
    std::vector<Client *>::iterator iter = mClients.begin();

    while (iter != mClients.end())
    {
        if ((*iter)->GetNickName() != requestorNickName)
            (*iter)->addResponseToBuf(message);
        iter++;
    }
}

// TODO: dahkang mOpeartor 벡터는 순회하지 않고 있음.
bool Channel::CheckClientIsExist(const std::string &nickName)
{
    std::vector<Client *>::iterator iter = mClients.begin();

    while (iter != mClients.end())
    {
        if ((*iter)->GetNickName() == nickName)
            return true;

        iter++;
    }

    return false;
}

bool Channel::CheckClientIsOperator(const std::string &nickName)
{
    std::vector<Client *>::iterator iter = mOperators.begin();

    while (iter != mOperators.end())
    {
        if ((*iter)->GetNickName() == nickName)
            return true;

        iter++;
    }

    return false;
}

void Channel::SetClient(Client *newClient)
{
    mClients.push_back(newClient);
}

void Channel::RemoveClient(const std::string &nickName)
{
    if (CheckClientIsOperator(nickName))
        RemoveOperator(nickName);

    std::vector<Client *>::iterator iter;

    iter = mClients.begin();
    while (iter != mClients.end())
    {
        if ((*iter)->GetNickName() == nickName)
        {
            mClients.erase(iter);
            break;
        }

        iter++;
    }
}

void Channel::SetOperator(Client *newOperator)
{
    mOperators.push_back(newOperator);
}

void Channel::RemoveOperator(const std::string &nickName)
{
    std::vector<Client *>::iterator iter;

    iter = mOperators.begin();
    while (iter != mOperators.end())
    {
        if ((*iter)->GetNickName() == nickName)
        {
            mOperators.erase(iter);
            break;
        }

        iter++;
    }
}

bool Channel::CheckClientIsInvited(const std::string &nickName)
{
    std::vector<Client *>::iterator iter;

    iter = mInvitedClients.begin();
    while (iter != mInvitedClients.end())
    {
        if ((*iter)->GetNickName() == nickName)
            return true;

        iter++;
    }

    return false;
}

void Channel::AddToInvitedClient(Client *invitedClient)
{
    mInvitedClients.push_back(invitedClient);
}

void Channel::RemoveFromInvitedClient(const std::string &nickName)
{
    std::vector<Client *>::iterator iter;

    iter = mInvitedClients.begin();
    while (iter != mInvitedClients.end())
    {
        if ((*iter)->GetNickName() == nickName)
        {
            mInvitedClients.erase(iter);
            break;
        }

        iter++;
    }
}

void Channel::SetTopic(const std::string &topic)
{
    mTopic = topic;
}

const std::string &Channel::GetTopic() const
{
    return mTopic;
}

const std::string &Channel::GetKey() const
{
    return mKey;
}

bool Channel::IsInviteOnlyMode() const
{
    return (mModeFlags & INVITE_ONLY_FLAG) != 0;
}

bool Channel::IsProtectedTopicMode() const
{
    return (mModeFlags & PROTECTED_TOPIC_FLAG) != 0;
}

bool Channel::IsKeyMode() const
{
    return (mModeFlags & KEY_FLAG) != 0;
}

bool Channel::IsClientLimitMode() const
{
    return (mModeFlags & CLIENT_LIMIT_FLAG) != 0;
}

// TODO: dahkang ^= 로 키면 다른 플래그가 꺼질 수 있음. |= 로 수정해야할듯.
void Channel::ToggleInviteOnlyMode()
{
    mModeFlags ^= INVITE_ONLY_FLAG;
}

void Channel::ToggleProtectedTopicMode()
{
    mModeFlags ^= PROTECTED_TOPIC_FLAG;
}

void Channel::ToggleKeyMode()
{
    mModeFlags ^= KEY_FLAG;
}

void Channel::ToggleClientLimitMode()
{
    mModeFlags ^= CLIENT_LIMIT_FLAG;
}

std::ostream &operator<<(std::ostream &os, const Channel &channel)
{
    os << "Channel = { Name: " << channel.mName << ", Clients.size: " << channel.mClients.size()
       << ", Operators.size: " << channel.mOperators.size() << ", Topic: " << channel.mTopic
       << ", ModeFlags: " << channel.mModeFlags << " }";

    return os;
}