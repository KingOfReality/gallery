#pragma once
#include "sqlite3.h"
#include "IDataAccess.h"
class DataBaseAccess : public IDataAccess
{
public:
	// album related
	const std::list<Album> getAlbums() override;
	const std::list<Album> getAlbumsOfUser(const User& user) override;
	const int getNextId();
	void createAlbum(const Album& album) override;
	void deleteAlbum(const std::string& albumName, int userId) override;
	bool doesAlbumExists(const std::string& albumName, int userId) override;
	Album openAlbum(const std::string& albumName) override;
	void closeAlbum(Album &pAlbum) override;
	void printAlbums() override;

	// picture related
	void addPictureToAlbumByName(const std::string& albumName, const Picture& picture) override;
	void removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName) override;
	void tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) override;
	void untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) override;
	void tagPictureFromdb();
	// user related
	const std::list<User> getUsers() ;
	void printUsers() override;
	void createUser(User& user) override;
	void deleteUser(const User& user) override;
	bool doesUserExists(int userId) override;
	User getUser(int userId) override;

	// user statistics
	int countAlbumsOwnedOfUser(const User& user) override;
    int countAlbumsTaggedOfUser(const User& user) override;
	int countTagsOfUser(const User& user) override;
	float averageTagsPerAlbumOfUser(const User& user) override;

	// queries
	User getTopTaggedUser() override;
	Picture getTopTaggedPicture() override;
	std::list<Picture> getTaggedPicturesOfUser(const User& user) override;
	//callbacks

	static int callback_getPictures(void* data, int argc, char** argv, char** azColName);
	static int callback_getTags(void* data, int argc, char** argv, char** azColName);
	static int callback_getAlbums(void* data, int argc, char** argv, char** azColName);
	static int callback_getUsers(void* data, int argc, char** argv, char** azColName);
	static int callback_getAlbumByUser(void* data, int argc, char** argv, char** azColName);
	static int callback_getAlbumByName(void* data, int argc, char** argv, char** azColName);
	static int callback_print(void* data, int argc, char** argv, char** azColName);
	static int callback_printUsers(void* data, int argc, char** argv, char** azColName);
	static int callback_GetUser(void* data, int argc, char** argv, char** azColName);
	static int callback_getCount(void* data, int argc, char** argv, char** azColName);
	static int callback_getCountTag(void* data, int argc, char** argv, char** azColName);
	static int callback_getAvg(void* data, int argc, char** argv, char** azColName);
	static int callback_TopTaggedUser(void* data, int argc, char** argv, char** azColName);
	 

	bool open() override;
	void close() override;
	void clear() override;
private:
	sqlite3* db;
	std::list<Album> m_albums;
	std::list<User> m_users;
};