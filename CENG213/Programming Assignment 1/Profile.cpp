#include "Profile.h"

Profile::Profile(const std::string &email, const std::string &username, SubscriptionPlan plan) {
    this->email = email;
    this->username = username;
    this->plan = plan;
}

const std::string &Profile::getUsername() const {
    return this->username;
}

const std::string &Profile::getEmail() const {
    return this->email;
}

SubscriptionPlan Profile::getPlan() const {
    return this->plan;
}

LinkedList<Playlist> &Profile::getPlaylists() {
    return this->playlists;
}

LinkedList<Profile *> &Profile::getFollowings() {
    return this->following;
}

LinkedList<Profile *> &Profile::getFollowers() {
    return this->followers;
}

void Profile::setPlan(SubscriptionPlan plan) {
    this->plan = plan;
}

void Profile::followProfile(Profile *profile) {
    following.insertAtTheEnd(profile);
    profile->getFollowers().insertAtTheEnd(this);
}

void Profile::unfollowProfile(Profile *profile) {
    following.removeNode(profile);
    profile->getFollowers().removeNode(this);
}

void Profile::createPlaylist(const std::string &playlistName) {
    playlists.insertAtTheEnd(playlistName);
}

void Profile::deletePlaylist(int playlistId) {
    playlists.removeNode(this->getPlaylist(playlistId)->getName());
}

void Profile::addSongToPlaylist(Song *song, int playlistId) {
    Playlist* searched_playlist = this->getPlaylist(playlistId);
    searched_playlist->addSong(song);
}

void Profile::deleteSongFromPlaylist(Song *song, int playlistId) {
    Playlist* searched_playlist = this->getPlaylist(playlistId);
    searched_playlist->dropSong(song);
}

Playlist *Profile::getPlaylist(int playlistId) {
    int i,n = playlists.getSize();
    Node<Playlist>* searched_playlist = NULL;
    for (i=0;i<n;i++){
        Node<Playlist>* current_playlist = playlists.getNodeAtIndex(i);
        if (current_playlist->data.getPlaylistId() == playlistId){
            searched_playlist = current_playlist;
            break;
        }
    }
    return  &(searched_playlist->data);
}

LinkedList<Playlist *> Profile::getSharedPlaylists() {
    LinkedList<Playlist*> shared_playlists;
    int i,n_followings = this->getFollowings().getSize();
    for (i=0;i<n_followings;i++){
        Node<Profile*>* current_following = this->getFollowings().getNodeAtIndex(i);
        int j,n_playlists_of_current_following = current_following->data->getPlaylists().getSize();
        for (j=0;j<n_playlists_of_current_following;j++){
            if (current_following->data->getPlaylists().getNodeAtIndex(j)->data.isShared() == true){
                shared_playlists.insertAtTheEnd(&(current_following->data->getPlaylists().getNodeAtIndex(j)->data));
            }
        }
    }
    return shared_playlists;
}

void Profile::shufflePlaylist(int playlistId, int seed) {
    Playlist* searched_playlist = this->getPlaylist(playlistId);
    searched_playlist->shuffle(seed);
}

void Profile::sharePlaylist(int playlistId) {
    Playlist* searched_playlist = this->getPlaylist(playlistId);
    searched_playlist->setShared(true);
}

void Profile::unsharePlaylist(int playlistId) {
    Playlist* searched_playlist = this->getPlaylist(playlistId);
    searched_playlist->setShared(false);
}

bool Profile::operator==(const Profile &rhs) const {
    return this->email == rhs.email && this->username == rhs.username && this->plan == rhs.plan;
}

bool Profile::operator!=(const Profile &rhs) const {
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Profile &profile) {
    os << "email: " << profile.email << " |";
    os << " username: " << profile.username << " |";
    if (profile.plan == free_of_charge) {
        os << " plan: " << "free_of_charge" << " |";
    } else if (profile.plan == premium) {
        os << " plan: " << "premium" << " |";
    } else {
        os << " plan: " << "undefined" << " |";
    }

    os << " playlists: [";
    Node<Playlist> *firstPlaylistNode = profile.playlists.getFirstNode();
    Node<Playlist> *playlistNode = firstPlaylistNode;
    if (playlistNode) {
        do {
            os << playlistNode->data;
            if (playlistNode->next != firstPlaylistNode) os << ", ";
            playlistNode = playlistNode->next;
        } while (playlistNode != firstPlaylistNode);
    }
    os << "] |";
    os << " following: [";
    Node<Profile *> *firstProfileNode = profile.following.getFirstNode();
    Node<Profile *> *profileNode = firstProfileNode;
    if (profileNode) {
        do {
            os << profileNode->data->getEmail();
            if (profileNode->next != firstProfileNode) os << ", ";
            profileNode = profileNode->next;
        } while (profileNode != firstProfileNode);
    }
    os << "] |";
    os << " followers: [";
    firstProfileNode = profile.followers.getFirstNode();
    profileNode = firstProfileNode;
    if (profileNode) {
        do {
            os << profileNode->data->getEmail();
            if (profileNode->next != firstProfileNode) os << ", ";
            profileNode = profileNode->next;
        } while (profileNode != firstProfileNode);
    }
    os << "]";

    return os;
}
