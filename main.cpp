#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

using namespace std;

ino_t getCurrentInode(void);
int toParentDir(void);
int inodeNumToPath(ino_t,string&);

int main(int argc,char* argv[])
{
    string path="";

    ino_t currentInode;
    currentInode=getCurrentInode();


    if(inodeNumToPath(currentInode,path)!=0){
        return -2;
    }

    while(toParentDir()==0){

        currentInode=getCurrentInode();


        if(inodeNumToPath(currentInode,path)!=0){
            return -2;
        }
       // cout<<path;

    }

    cout<<path;


    return 0;
}

int toParentDir(void){

    const char*parentDirName="..";
    ino_t oldInode=getCurrentInode();
    if(chdir(parentDirName)==-1){
        cout<<"ERROR Cannot Change Dir!"<<endl;
        return -1;
    };
    ino_t newInode=getCurrentInode();
    if (newInode!=oldInode) return 0;
    else return -2;

}



ino_t getCurrentInode(void){
    string pathname=".";

    struct stat statBuffer;


    ino_t inodeNumber;
    if((stat(pathname.c_str(),&statBuffer))==-1){
        cout<<"ERROR:Failed To Get Inode Number!"<<endl;
        cout<<"Maybe:Dir Not Exist!"<<endl;
        return -1;
    }
    else{
        inodeNumber=statBuffer.st_ino;
        return inodeNumber;
    }
}

int inodeNumToPath(ino_t inode,string& path){
    string temp;
    DIR *ptrDir;
    if((ptrDir=opendir(".."))==nullptr){
        cout<<"ERROR:Cannot Open Dir!"<<endl;
        return -1;
    }

    dirent *ptrDirent;
    while((ptrDirent=readdir(ptrDir))!=nullptr){
        if (ptrDirent->d_ino==inode){
            temp+=string(ptrDirent->d_name);
            path=temp+path;
            closedir(ptrDir);
            return 0;
        }
    }
    closedir(ptrDir);
    cout<<"ERROR:Cannot Find Inode!";
    return -3;


}








