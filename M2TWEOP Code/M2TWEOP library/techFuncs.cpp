#include "pch.h"
#include "techFuncs.h"

#include "miniz.h"
#include <filesystem>
#include "globals.h"
#include "smallFuncs.h"
void techFuncs::WriteData(void* ptr, DWORD to, size_t size)
{
    HANDLE h = GetCurrentProcess();
    DWORD oldMemProtect = 0;
    VirtualProtectEx(h, (LPVOID)to, size, PAGE_EXECUTE_READWRITE, &oldMemProtect);
    WriteProcessMemory(h, (LPVOID)to, ptr, size, NULL);
    VirtualProtectEx(h, (LPVOID)to, size, oldMemProtect, &oldMemProtect);

    CloseHandle(h);
}
void techFuncs::NopBytes(DWORD address, size_t size)
{
    std::vector<uint8_t> nops(size, 0x90);
    WriteData(nops.data(), address, size);
}
std::vector<std::string>techFuncs::unzip(std::string const& zipFile, std::string const& path)
{
    filesystem::path fPath = path;
    std::vector<std::string> files = {};
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));

    auto status = mz_zip_reader_init_file(&zip_archive, zipFile.c_str(), 0);
    if (!status) return files;
    int fileCount = (int)mz_zip_reader_get_num_files(&zip_archive);
    if (fileCount == 0)
    {
        mz_zip_reader_end(&zip_archive);
        return files;
    }
    mz_zip_archive_file_stat file_stat;
    if (!mz_zip_reader_file_stat(&zip_archive, 0, &file_stat))
    {
        mz_zip_reader_end(&zip_archive);
        return files;
    }
    // Get root folder
    string lastDir = "";
    string base = file_stat.m_filename; // path delim on end
    base += "\\";
    //  string base = slash_path(get_path(file_stat.m_filename)); // path delim on end

      // Get and print information about each file in the archive.
    for (int i = 0; i < fileCount; i++)
    {
        if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) continue;
        if (mz_zip_reader_is_file_a_directory(&zip_archive, i)) continue; // skip directories for now
        auto fileName = filesystem::path(file_stat.m_filename); // make path relative

        auto destFile = fPath;
        destFile.append(fileName.string()).string();

        //string destFile = combine_path(path, fileName); // make full dest path
        auto newDir = fileName.remove_filename(); // get the file's path
        if (newDir != lastDir)
        {
            auto newD = fPath;
            newD.append(newDir.string());

            if (!create_directory(newD)) // creates the directory
            {

            }
        }

        if (mz_zip_reader_extract_to_file(&zip_archive, i, destFile.string().c_str(), 0))
        {
            files.emplace_back(destFile.string());
        }
    }

    // Close the archive, freeing any resources it was using
    mz_zip_reader_end(&zip_archive);
    return files;
}

void techFuncs::zip(std::string const& zipFile, std::vector<std::string>& files, std::string saveFile, std::string nameOfSaveFile)
{
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));
    mz_bool status = mz_zip_writer_init_file(&zip_archive, zipFile.c_str(), 0);
    if (!status)
    {
        MessageBoxA(NULL, "Cannot create M2TWEOP save file.", "ERROR", NULL);
        exit(0);
    }


    for (string& file : files)
    {
        status = mz_zip_writer_add_file(&zip_archive, filesystem::path(file).filename().string().c_str(), file.c_str(), nullptr, 0, MZ_DEFAULT_COMPRESSION);
        if (!status)
        {
            MessageBoxA(NULL, "Cannot create M2TWEOP save file.", "ERROR", NULL);
            exit(0);
        }
    }
    status = mz_zip_writer_add_file(&zip_archive, nameOfSaveFile.c_str(), saveFile.c_str(), nullptr, 0, MZ_DEFAULT_COMPRESSION);
    if (!status)
    {
        MessageBoxA(NULL, "Cannot create M2TWEOP save file.", "ERROR", NULL);
        exit(0);
    }

    status = mz_zip_writer_finalize_archive(&zip_archive);
    if (!status)
    {
        MessageBoxA(NULL, "Cannot create M2TWEOP save file.", "ERROR", NULL);
        exit(0);
    }

    status = mz_zip_writer_end(&zip_archive);
    if (!status)
    {
        MessageBoxA(NULL, "Cannot create M2TWEOP save file.", "ERROR", NULL);
        exit(0);
    }
}

std::wstring techFuncs::ConvertUtf8ToWide(const std::string& str)
{
    int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
    return wstr;
}


string techFuncs::uniToANSI(UNICODE_STRING**& uniStr)
{
    if (uniStr == nullptr || *uniStr == nullptr)
    {
        return "";
    }
	UNICODE_STRING* uniS = *uniStr;
	wchar_t* wstr = (wchar_t*)&uniS->Buffer;

	std::string strTo;
    int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);

	char* szTo = new char[size_needed];

	WideCharToMultiByte(CP_ACP, 0, wstr, -1, szTo, size_needed, NULL, NULL);
    szTo[uniS->Length] = '\0';
	strTo = szTo;
	delete[] szTo;

	return strTo;
}


void techFuncs::deleteFiles(vector<string>& files)
{
    for (string& file : files)
    {
        filesystem::remove(filesystem::path(file));
    }
}




void techFuncs::saveGameMakeArchive(UNICODE_STRING**& savePath, vector<string>& files)
{
    string relativePath = uniToANSI(savePath);
    string packPath = globals::dataS.modPatch;



    packPath += "\\youneuoy_Data";


    if (!create_directory(filesystem::path(packPath))) // creates the directory
    {

    }
    packPath += "\\m2tweop_temp";
    if (!create_directory(filesystem::path(packPath))) // creates the directory
    {

    }
    packPath += "\\save";
    if (!create_directory(filesystem::path(packPath))) // creates the directory
    {

    }
    packPath += "\\";
    filesystem::path p = relativePath;
    packPath += p.filename().string();

    vector<string> zipfiles = files;



    zip(packPath, zipfiles, relativePath,"M2TWEOPTEMPgameSaveDONTTOUCHTHISFILE.sav");

    bool isRemoved=filesystem::remove(filesystem::path(relativePath));
    bool isCopied=filesystem::copy_file(filesystem::path(packPath), filesystem::path(relativePath));


}

vector<string> techFuncs::loadGameLoadArchive(UNICODE_STRING**& savePath)
{
	vector<string> archiveFiles;

    string relativePath = uniToANSI(savePath);
    if (std::filesystem::exists(relativePath) == false)
    {
        return archiveFiles;
    }
    string unpackPath = globals::dataS.modPatch;


    unpackPath += "\\youneuoy_Data";


    if (!create_directory(filesystem::path(unpackPath))) // creates the directory
    {

    }
    unpackPath += "\\m2tweop_temp";
    if (!create_directory(filesystem::path(unpackPath))) // creates the directory
    {

    }
    unpackPath += "\\save";
    if (!create_directory(filesystem::path(unpackPath))) // creates the directory
    {

    }

    archiveFiles=unzip(relativePath, unpackPath);
    if (archiveFiles.size() == 0)
    {
        MessageBoxA(NULL, "You trying load standart game save with M2TWEOP. Don`t do this, EOP is not full savecompatible with it.", "ERROR", NULL);
        exit(0);
    }

    for (string& file : archiveFiles)
    {
        if (filesystem::path(file).filename()=="M2TWEOPTEMPgameSaveDONTTOUCHTHISFILE.sav")
        {
  
            filesystem::path p = relativePath;

            p=p.parent_path();
            p=p.parent_path();

            p += "/saves/M2TWEOPgameSaveDONTTOUCHTHISFILE.tmp";
            filesystem::remove(p);
            filesystem::copy_file(file, p);
            smallFuncs::createUniString(savePath,p.string().c_str());
          //  (*savePath)->something = 1;
        }
    }





    return archiveFiles;
}
