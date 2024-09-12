#pragma once


#include <wx/wx.h>
#include <wx/process.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <string>

bool TestExecutable(const std::string& execName,
    const std::string& inputPath,
    const std::string& outputPath);