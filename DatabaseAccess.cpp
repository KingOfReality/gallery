#include "DatabaseAccess.h"
#include <string>
#include <vector>
#include <map>
#include "MyException.h"
int DataBaseAccess::callback_getPictures(void* data, int argc, char** argv, char** azColName)
{
    
    Picture temp(atoi(argv[0]),argv[1],argv[2],argv[3]);
    Album* a = (Album*)data;
    a->addPicture(temp);
    return 0;
}
int DataBaseAccess::callback_getTags(void* data, int argc, char** argv, char** azColName)
{
    ((Picture*)data)->tagUser(atoi(argv[1]));
    return 0;
}
int DataBaseAccess::callback_getAlbums(void* data, int argc, char** argv, char** azColName)
{
    Album temp;
    temp.setId(atoi(argv[0]));
    temp.setName(argv[1]);
    temp.setOwner(atoi(argv[2]));
    temp.setCreationDate(argv[3]);
   
    ((DataBaseAccess*)data)->m_albums.push_back(temp);
    return 0;
}
int DataBaseAccess::callback_getUsers(void* data, int argc, char** argv, char** azColName)
{
    std::list<User>* t = (std::list<User>*)data;
    User temp(atoi(argv[0]),argv[1]);
    t->push_back(temp);
    return 0;
}
int DataBaseAccess::callback_getAlbumByUser(void* data, int argc, char** argv, char** azColName)
{
    Album temp;
    temp.setName(argv[1]);
    temp.setOwner(atoi(argv[2]));
    temp.setCreationDate(argv[3]);

    ((DataBaseAccess*)data)->m_albums.push_back(temp);
    return 0;
}
int DataBaseAccess::callback_getAlbumByName(void* data, int argc, char** argv, char** azColName)
{
    ((Album*)data)->setName(argv[1]);
    ((Album*)data)->setOwner(atoi(argv[2]));
    ((Album*)data)->setCreationDate(argv[3]);
    return 0;
}
int DataBaseAccess::callback_print(void* data, int argc, char** argv, char** azColName)
{
    for (int i = 0; i < argc; i++)
    {
        std::cout << azColName[i] << " = " << argv[i] << " , ";
    }
    std::cout << std::endl<<std::endl<<std::endl;
    return 0;
}
int DataBaseAccess::callback_printUsers(void* data, int argc, char** argv, char** azColName)
{
    for (int i = 0; i < argc; i++)
    {
        std::cout << azColName[i] << " = " << argv[i] << " , ";
    }
    std::cout << std::endl;
    return 0;
}
int DataBaseAccess::callback_GetUser(void* data, int argc, char** argv, char** azColName)
{
    data = new User(atoi(argv[0]), argv[1]);
    return 0;
}
int DataBaseAccess::callback_getCount(void* data, int argc, char** argv, char** azColName)
{
    (int(data))*+1;
    return 0;
}
int DataBaseAccess::callback_getCountTag(void* data, int argc, char** argv, char** azColName)
{
    return 0;
}
int DataBaseAccess::callback_getAvg(void* data, int argc, char** argv, char** azColName)
{
    int num = 0;
    return 0;
}
int DataBaseAccess::callback_TopTaggedUser(void* data, int argc, char** argv, char** azColName)
{

    return 0;
}
bool DataBaseAccess::open()
{
    std::string dbFileName = "galleryDB.sqlite";
    int res = sqlite3_open(dbFileName.c_str(), &(this->db));
    if (res != SQLITE_OK)
    {
        std::cout << 111111111111;
        db = nullptr;
        return false;
    }
    getUsers();
    getAlbums();
    tagPictureFromdb();
    return true;
}

void DataBaseAccess::close()
{
    sqlite3_close(db);
    db = nullptr;
}

void DataBaseAccess::clear()
{
}

const std::list<Album> DataBaseAccess::getAlbums()
{
    const char* sqlStatement = "SELECT * FROM ALBUMS;";
    char* errMessage = nullptr;
    int res = sqlite3_exec(db, sqlStatement, callback_getAlbums, this, &errMessage);
    if (res != SQLITE_OK) {
        std::cout << 1;
    }
    for ( auto& album : this->m_albums)
    {
      std::string temp = "SELECT * FROM PICTURES\nWHERE ALBUM_ID='";
      temp += std::to_string(album.getId())+"';";
      sqlStatement = temp.c_str();
      errMessage = nullptr;
      res = sqlite3_exec(db, sqlStatement, callback_getPictures, &album, &errMessage);
      if (res != SQLITE_OK) {
          std::cout << errMessage;
      }
      
    }
    
    
    return this->m_albums;
}

const std::list<Album> DataBaseAccess::getAlbumsOfUser(const User& user)
{
    std::list<Album>::iterator itr;
    std::list<Album> temp;
    for (itr = this->m_albums.begin(); itr != this->m_albums.end(); itr++)
    {
        if (itr->getOwnerId() == user.getId())
            temp.push_back(*itr);
    }
    return temp;
}

const int DataBaseAccess::getNextId()
{
    return this->m_albums.size() + 1;
}
int retNextId(std::list<Album> m_album)
{
    int max = 1;
    for (auto& album : m_album)
    {
        if (album.getId() > max)
            return max;
        max++;
    }
    return m_album.size() + 1;
}
void DataBaseAccess::createAlbum(const Album& album)
{
    std::string sq = "INSERT INTO ALBUMS ('ID', 'NAME', 'USER_ID', 'CREATION_DATE')\n";
    sq += "VALUES ('";
    sq += std::to_string(retNextId(this->m_albums)) +"','";
    sq += album.getName();
    sq += "', ";
    sq+= album.getOwnerId()+48;
    sq += ", " + album.getCreationDate()+");";
    const char* sqlStatement = sq.c_str();
    char* errMessage = nullptr;
    int res = sqlite3_exec(this->db, sqlStatement, callback_getAlbums, this, &errMessage);
    if (res != SQLITE_OK) {
        std::cout << errMessage<<std::endl;
        std::cout << sqlStatement;
       
    }
}

void DataBaseAccess::deleteAlbum(const std::string& albumName, int userId)
{
    std::list<Album>::iterator itr;
    std::list<Album> temp;
    for (itr = this->m_albums.begin(); itr != this->m_albums.end(); itr++)
    {
        if (itr->getName() == albumName && itr->getOwnerId() == userId)
        {
            std::string sq = "DELETE FROM ALBUMS WHERE NAME=";
            sq += "'"+  albumName;
            sq += "'";
            
            const char* sqlStatement = sq.c_str();
            char* errMessage = nullptr;
            int res = sqlite3_exec(this->db, sqlStatement, callback_getAlbums, this, &errMessage);
            if (res != SQLITE_OK) {
                std::cout << errMessage << std::endl;
                std::cout << sqlStatement;
            }
            itr = this->m_albums.erase(itr);
        }
    }
}

bool DataBaseAccess::doesAlbumExists(const std::string& albumName, int userId)
{
    std::list<Album>::iterator itr;
    for (itr = this->m_albums.begin(); itr != this->m_albums.end(); itr++)
    {
        if (itr->getName() == albumName && itr->getOwnerId() == userId)
            return true;
    }
    return false;
}

Album DataBaseAccess::openAlbum(const std::string& albumName)
{
    
        for (const auto& album : this->m_albums)
        {
            if (album.getName() == albumName)
                return album;
        }
        throw("no");
}

void DataBaseAccess::closeAlbum(Album& pAlbum)
{
}

void DataBaseAccess::printAlbums()
{
    std::list<Picture>::iterator itr;
    char* sqlStatement = "SELECT * FROM ALBUMS";
    char* errMessage = nullptr;
    int res = sqlite3_exec(db, sqlStatement, callback_print, nullptr, &errMessage);
    if (res != SQLITE_OK) {
        std::cout << 1;
    }
    /*
    for (const auto& album : this->m_albums)
    {
        for (const auto& picture : this->m_albums.begin()->getPictures())
        {
            std::cout << picture;
        }   
    }*/
    
}

void DataBaseAccess::addPictureToAlbumByName(const std::string& albumName, const Picture& picture)
{
    int num = 0;
    std::list<Album>::iterator itr;
    
    std::string sq = "INSERT INTO PICTURES ('ID', 'NAME', 'LOCATION', 'CREATION_DATE','ALBUM_ID')\n";
    sq += "VALUES ('";
    sq += std::to_string(picture.getId()) + "','";
    sq += picture.getName() + "','";
    sq += picture.getPath() + "','";
    sq += picture.getCreationDate() + "','";
    sq += std::to_string(num) + "');";
    const char* sqlStatement = sq.c_str();
    char* errMessage = nullptr;
    int res = sqlite3_exec(this->db, sqlStatement, callback_getAlbums, this, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cout << errMessage;
        std::cout << sqlStatement;
    }
}

void DataBaseAccess::removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName)
{
    for (auto& album : this->m_albums)
    {
        if (album.getName() == albumName)
        {
            for (auto& picture : album.getPictures())
            {
                if (picture.getName() == pictureName)
                {
                    album.removePicture(pictureName);
                    break;
                }
            }
        }
    }
    std::string sq = "DELETE FROM PICTURES WHERE NAME='";
    sq += pictureName + "';";
    const char* sqlStatement = sq.c_str();
    char* errMessage = nullptr;
    int res = sqlite3_exec(this->db, sqlStatement, callback_getAlbums, this, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cout << errMessage;
        std::cout << sqlStatement;
    }
}

void DataBaseAccess::tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
   
    for (auto& album: this->m_albums)
    {
        if (album.getName() == albumName)
        {
            for (auto& image:album.getPictures())
            {
                if (image.getName() == pictureName)
                {
                    image.tagUser(userId);
                    album.removePicture(image.getName());
                    album.addPicture(image);
                }
            }
        }
    }
}

void DataBaseAccess::untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
   
    for (auto& album : this->m_albums)
    {
        if (album.getName() == albumName)
        {
            for (auto& image : album.getPictures())
            {
                if (image.getName() == pictureName)
                {
                    image.untagUser(userId);
                    album.removePicture(image.getName());
                    album.addPicture(image);
                }
            }
        }
    }
}

void DataBaseAccess::tagPictureFromdb()
{
    for (auto& album : this->m_albums)
    {
        for (auto& image : album.getPictures())
        {
            std::string hello = image.getName();
            std::cout << image.getName();
            std::string temp = "SELECT * FROM TAGS\nWHERE PICTURE_ID='";
            temp += std::to_string(image.getId()) + "';";
            char* errMessage = nullptr;
            int res = sqlite3_exec(db, temp.c_str(), callback_getTags, &image, &errMessage);
            if (res != SQLITE_OK) {
                std::cout << errMessage;
            }
            album.removePicture(hello);
            album.addPicture(image);
        }
    }
}

const std::list<User> DataBaseAccess::getUsers()
{
    std::list<User> hello;
    char* sqlStatement = "SELECT * FROM USERS;";
    char* errMessage = nullptr;
    int res = sqlite3_exec(db, sqlStatement, callback_getUsers, &hello, &errMessage);
    this->m_users = hello;
    return this->m_users;
}

void DataBaseAccess::printUsers()
{
    char* sqlStatement = "SELECT * FROM USERS;";
    char* errMessage = nullptr;
    int res = sqlite3_exec(db, sqlStatement, callback_print, nullptr, &errMessage);

}

void DataBaseAccess::createUser(User& user)
{
    
    int num = 1;
    std::string sq = "INSERT INTO USERS ('ID', 'NAME')\n";
    sq += "VALUES ('";
    sq += std::to_string(user.getId()) += "','";
    sq += user.getName() + "');";
    const char* sqlStatement = sq.c_str();
    char* errMessage = nullptr;
    int res = sqlite3_exec(this->db, sqlStatement, callback_getAlbums, this, &errMessage);
    if(res != SQLITE_OK) {
        std::cout << errMessage << std::endl;
        std::cout << sqlStatement;
        
    }
    this->m_users.push_back(user);
}

void DataBaseAccess::deleteUser(const User& user)
{
    std::string state = "DELETE FROM USERS WHERE ID=";
    state += "'"+ std::to_string(user.getId());
    state += "';";
    char* errMessage = nullptr;
    int res = sqlite3_exec(db, state.c_str(), nullptr, nullptr, &errMessage);
    if (res != SQLITE_OK) {
        std::cout << errMessage << std::endl;
        std::cout << state;

    }
}

bool DataBaseAccess::doesUserExists(int userId)
{
    std::list<User>::iterator itr;
    for (itr = this->m_users.begin(); itr != this->m_users.end(); itr++)
    {
        if (itr->getId() == userId)
            return true;
    }
    return false;
}

User DataBaseAccess::getUser(int userId)
{
    std::list<User>::iterator itr;
    for (itr = this->m_users.begin(); itr != this->m_users.end(); itr++)
    {
        if (itr->getId() == userId)
            return *itr;
    }
    return User(0,"");
}

int DataBaseAccess::countAlbumsOwnedOfUser(const User& user)
{
    int albumsCount = 0;

    for (const auto& album : m_albums) {
        if (album.getOwnerId() == user.getId()) {
            ++albumsCount;
        }
    }
    return albumsCount;
}

int DataBaseAccess::countAlbumsTaggedOfUser(const User& user)
{
    int albumsCount = 0;

    for (const auto& album : m_albums) {
        const std::list<Picture>& pics = album.getPictures();

        for (const auto& picture : pics) {
            if (picture.isUserTagged(user)) {
                albumsCount++;
                break;
            }
        }
    }

    return albumsCount;
}

int DataBaseAccess::countTagsOfUser(const User& user)
{
    int tagsCount = 0;

    for (const auto& album : m_albums) {
        const std::list<Picture>& pics = album.getPictures();

        for (const auto& picture : pics) {
            if (picture.isUserTagged(user)) {
                tagsCount++;
            }
        }
    }

    return tagsCount;
}
float DataBaseAccess::averageTagsPerAlbumOfUser(const User& user)
{
    int albumsTaggedCount = countAlbumsTaggedOfUser(user);

    if (0 == albumsTaggedCount) {
        return 0;
    }

    return static_cast<float>(countTagsOfUser(user)) / albumsTaggedCount;
}

User DataBaseAccess::getTopTaggedUser()
{
    std::map<int, int> userTagsCountMap;

    auto albumsIter = m_albums.begin();
    for (const auto& album : m_albums) {
        for (const auto& picture : album.getPictures()) {

            const std::set<int>& userTags = picture.getUserTags();
            for (const auto& user : userTags) {
                //As map creates default constructed values, 
                //users which we haven't yet encountered will start from 0
                userTagsCountMap[user]++;
            }
        }
    }

    if (userTagsCountMap.size() == 0) {
        throw MyException("There isn't any tagged user.");
    }

    int topTaggedUser = -1;
    int currentMax = -1;
    for (auto entry : userTagsCountMap) {
        if (entry.second < currentMax) {
            continue;
        }

        topTaggedUser = entry.first;
        currentMax = entry.second;
    }

    if (-1 == topTaggedUser) {
        throw MyException("Failed to find most tagged user");
    }

    return getUser(topTaggedUser);
}

Picture DataBaseAccess::getTopTaggedPicture()
{
    int currentMax = -1;
    const Picture* mostTaggedPic = nullptr;
    for (const auto& album : m_albums) {
        for (const Picture& picture : album.getPictures()) {
            int tagsCount = picture.getTagsCount();
            if (tagsCount == 0) {
                continue;
            }

            if (tagsCount <= currentMax) {
                continue;
            }

            mostTaggedPic = &picture;
            currentMax = tagsCount;
        }
    }
    if (nullptr == mostTaggedPic) {
        throw MyException("There isn't any tagged picture.");
    }

    return *mostTaggedPic;
}

std::list<Picture> DataBaseAccess::getTaggedPicturesOfUser(const User& user)
{
    std::list<Picture> pictures;

    for (const auto& album : m_albums) {
        for (const auto& picture : album.getPictures()) {
            if (picture.isUserTagged(user)) {
                pictures.push_back(picture);
            }
        }
    }

    return pictures;
}




