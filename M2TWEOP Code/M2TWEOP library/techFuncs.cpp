#include "pch.h"
#include "techFuncs.h"

#include "miniz.h"

#include "functionsOffsets.h"
#include "gameHelpers.h"
#include "gameStringHelpers.h"
#include "globals.h"

void techFuncs::writeData(const void* ptr, const DWORD to, const size_t size)
{
    const HANDLE h = GetCurrentProcess();
    DWORD oldMemProtect = 0;
    VirtualProtectEx(h, reinterpret_cast<LPVOID>(to), size, PAGE_EXECUTE_READWRITE, &oldMemProtect);
    WriteProcessMemory(h, reinterpret_cast<LPVOID>(to), ptr, size, nullptr);
    VirtualProtectEx(h, reinterpret_cast<LPVOID>(to), size, oldMemProtect, &oldMemProtect);
    CloseHandle(h);
}

void techFuncs::nopBytes(const DWORD address, const size_t size)
{
    const std::vector<uint8_t> nops(size, 0x90);
    writeData(nops.data(), address, size);
}

std::vector<std::string>techFuncs::unzip(std::string const& zipFile, std::string const& path)
{
    filesystem::path fPath = path;
    std::vector<std::string> files = {};
    mz_zip_archive zipArchive = {};

    if (auto status = mz_zip_reader_init_file(&zipArchive, zipFile.c_str(), 0); !status)
        return files;
    int fileCount = static_cast<int>(mz_zip_reader_get_num_files(&zipArchive));
    if (fileCount == 0)
    {
        mz_zip_reader_end(&zipArchive);
        return files;
    }
    mz_zip_archive_file_stat fileStat;
    if (!mz_zip_reader_file_stat(&zipArchive, 0, &fileStat))
    {
        mz_zip_reader_end(&zipArchive);
        return files;
    }
    // Get root folder
    string base = fileStat.m_filename; // path delim on end
    base += "\\";

    // Get and print information about each file in the archive.
    for (int i = 0; i < fileCount; i++)
    {
        string lastDir;
        if (!mz_zip_reader_file_stat(&zipArchive, i, &fileStat)) continue;
        if (mz_zip_reader_is_file_a_directory(&zipArchive, i)) continue; // skip directories for now
        auto fileName = filesystem::path(fileStat.m_filename); // make path relative

        auto destFile = fPath;
        auto newPath = destFile.append(fileName.string()).string();
        //string destFile = combine_path(path, fileName); // make full dest path
        if (auto newDir = fileName.remove_filename(); newDir != lastDir)
        {
            auto newD = fPath;
            newD.append(newDir.string());
            if (!create_directory(newD)) // creates the directory
            {

            }
        }
        if (mz_zip_reader_extract_to_file(&zipArchive, i, destFile.string().c_str(), 0))
        {
            files.emplace_back(destFile.string());
        }
    }

    // Close the archive, freeing any resources it was using
    mz_zip_reader_end(&zipArchive);
    return files;
}

	
std::string techFuncs::readFile(const std::filesystem::path& path)
{
    // Open the stream to 'lock' the file.
    std::ifstream f(path, std::ios::in | std::ios::binary);
    // Obtain the size of the file.
    const auto sz = file_size(path);
    // Create a buffer.
    std::string result(static_cast<size_t>(sz), '\0');
    // Read the whole file into the buffer.
    f.read(result.data(), sz);
    return result;
}

void techFuncs::zip(std::string const& zipFile, std::vector<std::string>& files,
    const std::string& saveFile, const std::string& nameOfSaveFile)
{
    mz_zip_archive zipArchive = {};
    mz_bool status = mz_zip_writer_init_file(&zipArchive, zipFile.c_str(), 0);
    if (!status)
    {
        MessageBoxA(nullptr, "Cannot create M2TWEOP save file.", "ERROR", NULL);
        exit(0);
    }
    for (string& file : files)
    {
        gameHelpers::logStringGame("Adding file to archive: " + file);
        status = mz_zip_writer_add_file(&zipArchive, filesystem::path(file).filename().string().c_str(), file.c_str(), nullptr, 0, MZ_DEFAULT_COMPRESSION);
        if (!status)
        {
            gameHelpers::logStringGame("Cannot create M2TWEOP save file. 1");
            MessageBoxA(nullptr, "Cannot create M2TWEOP save file. 1", "ERROR", NULL);
            exit(0);
        }
    }
    status = mz_zip_writer_add_file(&zipArchive, nameOfSaveFile.c_str(), saveFile.c_str(), nullptr, 0, MZ_DEFAULT_COMPRESSION);
    if (!status)
    {
        gameHelpers::logStringGame("Cannot create M2TWEOP save file. 2");
        MessageBoxA(nullptr, "Cannot create M2TWEOP save file. 2", "ERROR", NULL);
        exit(0);
    }
    status = mz_zip_writer_finalize_archive(&zipArchive);
    if (!status)
    {
        gameHelpers::logStringGame("Cannot create M2TWEOP save file. 3");
        MessageBoxA(nullptr, "Cannot create M2TWEOP save file. 3", "ERROR", NULL);
        exit(0);
    }
    status = mz_zip_writer_end(&zipArchive);
    if (!status)
    {
        gameHelpers::logStringGame("Cannot create M2TWEOP save file. 4");
        MessageBoxA(nullptr, "Cannot create M2TWEOP save file. 4", "ERROR", NULL);
        exit(0);
    }
}

std::wstring techFuncs::convertUtf8ToWide(const std::string& str)
{
    const int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), nullptr, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), wstr.data(), count);
    return wstr;
}

namespace
{
    string uniToAcp(UNICODE_STRING**& uniStr)
    {
        if (uniStr == nullptr || *uniStr == nullptr)
            return "";
        UNICODE_STRING* uniS = *uniStr;
        const auto wstr = reinterpret_cast<wchar_t*>(&uniS->Buffer);
        const int sizeNeeded = WideCharToMultiByte(CP_ACP, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
        const auto szTo = new char[sizeNeeded];
        WideCharToMultiByte(CP_ACP, 0, wstr, -1, szTo, sizeNeeded, nullptr, nullptr);
        szTo[uniS->Length] = '\0';
        std::string strTo = szTo;
        delete[] szTo;
        return strTo;
    }
}

static bool checNonStandardChar(string text)
{
    if (text.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_/-.,") != std::string::npos)
    {
        return true;
    }
    return false;
}

string techFuncs::uniToAnsi(UNICODE_STRING**& uniStr)
{
    if (uniStr == nullptr || *uniStr == nullptr)
    {
        return "";
    }
    UNICODE_STRING* uniS = *uniStr;
    const auto wstr = reinterpret_cast<wchar_t*>(&uniS->Buffer);
    const int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
    const auto szTo = new char[sizeNeeded];
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, szTo, sizeNeeded, nullptr, nullptr);
    szTo[uniS->Length] = '\0';
    std::string strTo = szTo;
    delete[] szTo;

    // Russian save fix   
    if (checNonStandardChar(strTo))
    {
        strTo = uniToAcp(uniStr);
    }

    return strTo;
}

void techFuncs::deleteFiles(vector<string>& files)
{
    for (string& file : files)
    {
        try
        {
            if (filesystem::exists(file))
                filesystem::remove(filesystem::path(file));
        }
        catch (exception& e)
        {
            gameHelpers::logStringGame(e.what());
            continue;
        }
    }
}

void techFuncs::saveGameMakeArchive(UNICODE_STRING**& savePath, const vector<string>& files)
{
    const string relativePath = uniToAcp(savePath);
    string packPath = globals::dataS.modPath;
    packPath += "\\eopData";
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
    vector<string> zipFiles = files;
    zip(packPath, zipFiles, relativePath,"M2TWEOPTEMPgameSaveDONTTOUCHTHISFILE.sav");
    filesystem::remove(filesystem::path(relativePath));
    filesystem::copy_file(filesystem::path(packPath), filesystem::path(relativePath));
}

vector<string> techFuncs::getEopArchiveFiles(const string& savePath)
{
    vector<string> archiveFiles;
    if (std::filesystem::exists(savePath) == false)
        return archiveFiles;
    string unpackPath = globals::dataS.modPath;
    unpackPath += "\\eopData";
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
    archiveFiles = techFuncs::unzip(savePath, unpackPath);
    return archiveFiles;
}

DWORD techFuncs::allocateGameMem(size_t amount)
{
    DWORD retMem = 0;
    DWORD adrFunc = codes::offsets.allocMemFunc;
    _asm
    {
        push amount
        mov eax, adrFunc
        call eax
        add esp, 0x4
        mov retMem, eax
    }
    return retMem;
}


vector<string> techFuncs::loadGameLoadArchive(vector<string> files, UNICODE_STRING**& savePath)
{
    if (files.empty())
    {
        MessageBoxA(nullptr, "You have attempted to load a non-EOP save with EOP. This is not supported and the game will now exit. You can find instructions on converting a non-EOP save to an EOP save in our Discord.", "ERROR", NULL);
        exit(0);
    }
    const string relativePath = uniToAnsi(savePath);
    for (string& file : files)
    {
        if (filesystem::path(file).filename()=="M2TWEOPTEMPgameSaveDONTTOUCHTHISFILE.sav")
        {
            filesystem::path p = relativePath;
            p = p.parent_path();
            p = p.parent_path();
            p += "/saves/M2TWEOPgameSaveDONTTOUCHTHISFILE.tmp";
            filesystem::remove(p);
            filesystem::copy_file(file, p);
            gameStringHelpers::createUniString(savePath,p.string().c_str());
        }
    }
    return files;
}
