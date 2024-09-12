#include "../Header Files/TestExecutable.h"

bool TestExecutable(const std::string& execName,
    const std::string& inputPath,
    const std::string& outputPath)
{
    wxProcess process;
    process.Redirect();

    wxString command = "powershell -Command Get-Content ./" + inputPath + " | ./" + execName;
    int executionResult = wxExecute(command, wxEXEC_SYNC, &process);
    if (executionResult == -1)
    {
        wxMessageBox("Failed to execute program!", "Error", wxOK | wxICON_ERROR);
        return false;
    }

    wxInputStream* actualOutputStream = process.GetInputStream();
    wxFileInputStream outputFileStream(outputPath);
    wxInputStream* expectedOutputStream = &outputFileStream;
    if (actualOutputStream && actualOutputStream->IsOk() && expectedOutputStream && expectedOutputStream->IsOk())
    {
        wxTextInputStream inputText(*actualOutputStream);
        wxTextInputStream outputText(*expectedOutputStream);
        wxString output, inputTxt, outputTxt;
        bool flag = true;

        // Read the output line by line and compare with input
        while (!expectedOutputStream->Eof() && !actualOutputStream->Eof()) {
            inputTxt = inputText.ReadLine();
            outputTxt = outputText.ReadLine();
            output += "'" + inputTxt + "' : '" + outputTxt + "'\n";
            if (inputTxt != outputTxt) {
                flag = false;
                wxLogMessage("Comparing input: '%s' with output: '%s'", inputTxt, outputTxt);
            }
        }

        while (!expectedOutputStream->Eof()) {
            outputTxt = outputText.ReadLine().Trim(false).Trim(true);
            if (!outputTxt.IsEmpty()) {
                flag = false;
                wxLogMessage("Expected: '%s' but file ended.", outputTxt);
            }
        }

        while (!actualOutputStream->Eof()) {
            inputTxt = inputText.ReadLine().Trim(false).Trim(true);
            if (!inputTxt.IsEmpty()) {
                flag = false;
                wxLogMessage("Unexpected: '%s' when output ended.", inputTxt);
            }
        }

        return flag;
    }

    return false;
}