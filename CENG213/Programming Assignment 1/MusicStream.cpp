#include "MusicStream.h"

#include <iostream>

void MusicStream::addProfile(const std::string &email, const std::string &username, SubscriptionPlan plan) {
    profiles.insertAtTheEnd(Profile(email,username,plan));
}

void MusicStream::deleteProfile(const std::string &email) {
    // Find the profile with email.
    int i,n_profiles = this->profiles.getSize();
    Node<Profile>* profile_to_be_deleted;
    for (i=0;i<n_profiles;i++){
        if (this->profiles.getNodeAtIndex(i)->data.getEmail() == email){
            profile_to_be_deleted = this->profiles.getNodeAtIndex(i);
        }
    }
    
    // Remove the user from its followers' followings list.
    LinkedList<Profile*> followers_of_the_user = profile_to_be_deleted->data.getFollowers();
    int n_followers = followers_of_the_user.getSize();
    for (i=0;i<n_followers;i++){
        Node<Profile*>* current_follower = followers_of_the_user.getNodeAtIndex(i);
        this->unfollowProfile(current_follower->data->getEmail(),email);
    }
    
    // Remove the user from its followings' followers list.
    LinkedList<Profile*> followings_of_the_user = profile_to_be_deleted->data.getFollowings();
    int n_followings = followings_of_the_user.getSize();
    for (i=0;i<n_followings;i++){
        Node<Profile*>* current_following = followings_of_the_user.getNodeAtIndex(i);
        this->unfollowProfile(email,current_following->data->getEmail());
    }
    
    profile_to_be_deleted->data.getPlaylists().removeAllNodes();
    profile_to_be_deleted->data.getFollowers().removeAllNodes();
    profile_to_be_deleted->data.getFollowings().removeAllNodes();
    this->profiles.removeNode(profile_to_be_deleted);
}

void MusicStream::addArtist(const std::string &artistName) {
    artists.insertAtTheEnd(Artist(artistName));
}

void MusicStream::addAlbum(const std::string &albumName, int artistId) {
    int i,n_artists = this->artists.getSize();
    Node<Artist>* searched_artist;
    for (i=0;i<n_artists;i++){
        if (this->artists.getNodeAtIndex(i)->data.getArtistId() == artistId){
            searched_artist = this->artists.getNodeAtIndex(i);
            break;
        }
    }
    albums.insertAtTheEnd(Album(albumName));
    searched_artist->data.addAlbum(&(albums.getLastNode()->data));
}

void MusicStream::addSong(const std::string &songName, int songDuration, int albumId) {
    int i,n_albums = this->albums.getSize();
    Node<Album>* searched_album;
    for (i=0;i<n_albums;i++){
        if (this->albums.getNodeAtIndex(i)->data.getAlbumId() == albumId){
            searched_album = this->albums.getNodeAtIndex(i);
            break;
        }
    }
    songs.insertAtTheEnd(Song(songName,songDuration));
    searched_album->data.addSong(&(songs.getLastNode()->data));
}

void MusicStream::followProfile(const std::string &email1, const std::string &email2) {
    int i,n_profiles = this->profiles.getSize();
    Node<Profile>* profile1;
    Node<Profile>* profile2;
    for (i=0;i<n_profiles;i++){
        if (this->profiles.getNodeAtIndex(i)->data.getEmail() == email1){
            profile1 = this->profiles.getNodeAtIndex(i);
            break;
        }
    }
    for (i=0;i<n_profiles;i++){
        if (this->profiles.getNodeAtIndex(i)->data.getEmail() == email2){
            profile2 = this->profiles.getNodeAtIndex(i);
            break;
        }
    }
    profile1->data.followProfile(&(profile2->data));
}

void MusicStream::unfollowProfile(const std::string &email1, const std::string &email2) {
    int i,n_profiles = this->profiles.getSize();
    Node<Profile>* profile1;
    Node<Profile>* profile2;
    for (i=0;i<n_profiles;i++){
        if (this->profiles.getNodeAtIndex(i)->data.getEmail() == email1){
            profile1 = this->profiles.getNodeAtIndex(i);
            break;
        }
    }
    for (i=0;i<n_profiles;i++){
        if (this->profiles.getNodeAtIndex(i)->data.getEmail() == email2){
            profile2 = this->profiles.getNodeAtIndex(i);
            break;
        }
    }
    profile1->data.unfollowProfile(&(profile2->data));
}

void MusicStream::createPlaylist(const std::string &email, const std::string &playlistName) {
    int i,n_profiles = this->profiles.getSize();
    Node<Profile>* searched_profile;
    for (i=0;i<n_profiles;i++){
        if (this->profiles.getNodeAtIndex(i)->data.getEmail() == email){
            searched_profile = this->profiles.getNodeAtIndex(i);
            break;
        }
    }

    searched_profile->data.getPlaylists().insertAtTheEnd(Playlist(playlistName));
}

void MusicStream::deletePlaylist(const std::string &email, int playlistId) {
    int i,n_profiles = this->profiles.getSize();
    Node<Profile>* searched_profile;
    for (i=0;i<n_profiles;i++){
        if (this->profiles.getNodeAtIndex(i)->data.getEmail() == email){
            searched_profile = this->profiles.getNodeAtIndex(i);
            break;
        }
    }

    int n_playlists_of_searched_profile = searched_profile->data.getPlaylists().getSize();
    Node<Playlist>* searched_playlist;
    for (i=0;i<n_playlists_of_searched_profile;i++){
        if (searched_profile->data.getPlaylists().getNodeAtIndex(i)->data.getPlaylistId() == playlistId){
            searched_playlist = searched_profile->data.getPlaylists().getNodeAtIndex(i);
            break;
        }
    }

    searched_profile->data.getPlaylists().removeNode(searched_playlist);
}

void MusicStream::addSongToPlaylist(const std::string &email, int songId, int playlistId) {
    int i,n_profiles = this->profiles.getSize();
    Node<Profile>* searched_profile;
    for (i=0;i<n_profiles;i++){
        if (this->profiles.getNodeAtIndex(i)->data.getEmail() == email){
            searched_profile = this->profiles.getNodeAtIndex(i);
            break;
        }
    }

    int n_playlists_of_searched_profile = searched_profile->data.getPlaylists().getSize();
    Node<Playlist>* searched_playlist;
    for (i=0;i<n_playlists_of_searched_profile;i++){
        if (searched_profile->data.getPlaylists().getNodeAtIndex(i)->data.getPlaylistId() == playlistId){
            searched_playlist = searched_profile->data.getPlaylists().getNodeAtIndex(i);
            break;
        }
    }

    int n_songs = this->songs.getSize();
    Node<Song>* searched_song;
    for (i=0;i<n_songs;i++){
        if (this->songs.getNodeAtIndex(i)->data.getSongId() == songId){
            searched_song = this->songs.getNodeAtIndex(i);
            break;
        }
    }

    searched_playlist->data.addSong(&(searched_song->data));
}

void MusicStream::deleteSongFromPlaylist(const std::string &email, int songId, int playlistId) {
    int i,n_profiles = this->profiles.getSize();
    Node<Profile>* searched_profile;
    for (i=0;i<n_profiles;i++){
        if (this->profiles.getNodeAtIndex(i)->data.getEmail() == email){
            searched_profile = this->profiles.getNodeAtIndex(i);
            break;
        }
    }

    int n_playlists_of_searched_profile = searched_profile->data.getPlaylists().getSize();
    Node<Playlist>* searched_playlist;
    for (i=0;i<n_playlists_of_searched_profile;i++){
        if (searched_profile->data.getPlaylists().getNodeAtIndex(i)->data.getPlaylistId() == playlistId){
            searched_playlist = searched_profile->data.getPlaylists().getNodeAtIndex(i);
            break;
        }
    }

    int n_songs_of_searched_playlist = searched_playlist->data.getSongs().getSize();
    Node<Song*>* searched_song;
    for (i=0;i<n_songs_of_searched_playlist;i++){
        if (searched_playlist->data.getSongs().getNodeAtIndex(i)->data->getSongId() == songId){
            searched_song = searched_playlist->data.getSongs().getNodeAtIndex(i);
            break;
        }
    }

    searched_playlist->data.getSongs().removeNode(searched_song);
}

LinkedList<Song *> MusicStream::playPlaylist(const std::string &email, Playlist *playlist) {
    int i,n_profiles = this->profiles.getSize();
    Node<Profile>* searched_profile;
    for (i=0;i<n_profiles;i++){
        if (this->profiles.getNodeAtIndex(i)->data.getEmail() == email){
            searched_profile = this->profiles.getNodeAtIndex(i);
            break;
        }
    }

    if (searched_profile->data.getPlan() == premium){
        return playlist->getSongs();
    }
    else { // Plan is free_of_charge.
        LinkedList<Song*> playPlayList_withAdvertisement;
        int i,n_songs = playlist->getSongs().getSize();
        for (i=0;i<n_songs;i++){
            playPlayList_withAdvertisement.insertAtTheEnd(playlist->getSongs().getNodeAtIndex(i)->data);
            playPlayList_withAdvertisement.insertAtTheEnd(&(Song::ADVERTISEMENT_SONG));
        }
        return playPlayList_withAdvertisement;
    }
}

Playlist *MusicStream::getPlaylist(const std::string &email, int playlistId) {
    int i,n_profiles = this->profiles.getSize();
    Node<Profile>* searched_profile;
    for (i=0;i<n_profiles;i++){
        if (this->profiles.getNodeAtIndex(i)->data.getEmail() == email){
            searched_profile = this->profiles.getNodeAtIndex(i);
            break;
        }
    }

    int n_playlists_of_searched_profile = searched_profile->data.getPlaylists().getSize();
    Node<Playlist>* searched_playlist;
    for (i=0;i<n_playlists_of_searched_profile;i++){
        if (searched_profile->data.getPlaylists().getNodeAtIndex(i)->data.getPlaylistId() == playlistId){
            searched_playlist = searched_profile->data.getPlaylists().getNodeAtIndex(i);
            break;
        }
    }

    return &(searched_playlist->data);
    /* Check this function later. */
}

LinkedList<Playlist *> MusicStream::getSharedPlaylists(const std::string &email) {
    int i,n_profiles = this->profiles.getSize();
    Node<Profile>* searched_profile;
    for (i=0;i<n_profiles;i++){
        if (this->profiles.getNodeAtIndex(i)->data.getEmail() == email){
            searched_profile = this->profiles.getNodeAtIndex(i);
            break;
        }
    }

    return searched_profile->data.getSharedPlaylists();
}

void MusicStream::shufflePlaylist(const std::string &email, int playlistId, int seed) {
    int i,n_profiles = this->profiles.getSize();
    Node<Profile>* searched_profile;
    for (i=0;i<n_profiles;i++){
        if (this->profiles.getNodeAtIndex(i)->data.getEmail() == email){
            searched_profile = this->profiles.getNodeAtIndex(i);
            break;
        }
    }

    int n_playlists_of_searched_profile = searched_profile->data.getPlaylists().getSize();
    Node<Playlist>* searched_playlist;
    for (i=0;i<n_playlists_of_searched_profile;i++){
        if (searched_profile->data.getPlaylists().getNodeAtIndex(i)->data.getPlaylistId() == playlistId){
            searched_playlist = searched_profile->data.getPlaylists().getNodeAtIndex(i);
            break;
        }
    }

    searched_playlist->data.getSongs().shuffle(seed);
}

void MusicStream::sharePlaylist(const std::string &email, int playlistId) {
    int i,n_profiles = this->profiles.getSize();
    Node<Profile>* searched_profile;
    for (i=0;i<n_profiles;i++){
        if (this->profiles.getNodeAtIndex(i)->data.getEmail() == email){
            searched_profile = this->profiles.getNodeAtIndex(i);
            break;
        }
    }

    int n_playlists_of_searched_profile = searched_profile->data.getPlaylists().getSize();
    Node<Playlist>* searched_playlist;
    for (i=0;i<n_playlists_of_searched_profile;i++){
        if (searched_profile->data.getPlaylists().getNodeAtIndex(i)->data.getPlaylistId() == playlistId){
            searched_playlist = searched_profile->data.getPlaylists().getNodeAtIndex(i);
            break;
        }
    }

    searched_playlist->data.setShared(true);
}

void MusicStream::unsharePlaylist(const std::string &email, int playlistId) {
    int i,n_profiles = this->profiles.getSize();
    Node<Profile>* searched_profile;
    for (i=0;i<n_profiles;i++){
        if (this->profiles.getNodeAtIndex(i)->data.getEmail() == email){
            searched_profile = this->profiles.getNodeAtIndex(i);
            break;
        }
    }

    int n_playlists_of_searched_profile = searched_profile->data.getPlaylists().getSize();
    Node<Playlist>* searched_playlist;
    for (i=0;i<n_playlists_of_searched_profile;i++){
        if (searched_profile->data.getPlaylists().getNodeAtIndex(i)->data.getPlaylistId() == playlistId){
            searched_playlist = searched_profile->data.getPlaylists().getNodeAtIndex(i);
            break;
        }
    }

    searched_playlist->data.setShared(false);
}

void MusicStream::subscribePremium(const std::string &email) {
    int i,n_profiles = this->profiles.getSize();
    Node<Profile>* searched_profile;
    for (i=0;i<n_profiles;i++){
        if (this->profiles.getNodeAtIndex(i)->data.getEmail() == email){
            searched_profile = this->profiles.getNodeAtIndex(i);
            break;
        }
    }
    searched_profile->data.setPlan(premium);
}

void MusicStream::unsubscribePremium(const std::string &email) {
    int i,n_profiles = this->profiles.getSize();
    Node<Profile>* searched_profile;
    for (i=0;i<n_profiles;i++){
        if (this->profiles.getNodeAtIndex(i)->data.getEmail() == email){
            searched_profile = this->profiles.getNodeAtIndex(i);
            break;
        }
    }
    searched_profile->data.setPlan(free_of_charge);
}

void MusicStream::print() const {
    std::cout << "# Printing the music stream ..." << std::endl;

    std::cout << "# Number of profiles is " << profiles.getSize() << ":" << std::endl;
    profiles.print();

    std::cout << "# Number of artists is " << artists.getSize() << ":" << std::endl;
    artists.print();

    std::cout << "# Number of albums is " << albums.getSize() << ":" << std::endl;
    albums.print();

    std::cout << "# Number of songs is " << songs.getSize() << ":" << std::endl;
    songs.print();

    std::cout << "# Printing is done." << std::endl;
}