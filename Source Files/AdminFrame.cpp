#include "../Header Files/AdminFrame.h"



AdminFrame::AdminFrame(const wxString& title, User adminUser)
	: wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300))
{
	currentUser = std::move(adminUser);

	// Create the main panel
	wxPanel* admin_panel = new wxPanel(this, wxID_ANY);
	admin_panel->SetBackgroundColour(wxColour(240, 240, 240));  // Light gray background

	// Create buttons for "Create User" and "Delete User"
	wxButton* create_user_button = new wxButton(admin_panel, wxID_ANY, "Create User", wxDefaultPosition, wxSize(100, 30));
	wxButton* delete_user_button = new wxButton(admin_panel, wxID_ANY, "Delete User", wxDefaultPosition, wxSize(100, 30));

	// Bind events to the buttons
	create_user_button->Bind(wxEVT_BUTTON, &AdminFrame::onCreateUser, this);
	delete_user_button->Bind(wxEVT_BUTTON, &AdminFrame::onDeleteUser, this);
	Bind(wxEVT_CLOSE_WINDOW, &AdminFrame::OnClose, this);

	// Get the user list from the JSON file
	load_users();

	get_keys(usersMap, wxUserArray);

	// Create a styled wxListBox to display users
	userList = new wxListBox(admin_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxUserArray, wxLB_SINGLE | wxBORDER_SIMPLE);
	userList->SetBackgroundColour(wxColour(255, 255, 255));  // White background for the list
	userList->SetForegroundColour(wxColour(0, 0, 0));        // Black text color
	userList->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	// Use a sizer to manage layout
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(create_user_button, 0, wxRIGHT, 10);
	buttonSizer->Add(delete_user_button, 0);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->AddSpacer(20); // Add space at the top
	mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 20); // Align buttons to the right
	mainSizer->Add(userList, 1, wxALL | wxEXPAND, 20);  // Add the list with padding

	// Set the sizer for the panel
	admin_panel->SetSizer(mainSizer);
	admin_panel->Layout();  // Force the layout to update

	// Center the frame on the screen
	Centre();
}

void AdminFrame::OnClose(wxCloseEvent& event)
{
	// Convert the usersMap (or your data structure) back to JSON
	json j_users = json::array();
	j_users.push_back(currentUser);

	// Iterate over the map and add each user object to the array
	for (const auto& pair : usersMap) {
		j_users.push_back(pair.second);

		// Write the JSON to the file
		std::ofstream file("users-schema.json");
		if (file.is_open()) {
			file << j_users.dump(4);  // Dump the JSON with 4-space indentation
			file.close();
		}
		else {
			wxMessageBox("Failed to save user data to file.", "Error", wxOK | wxICON_ERROR);
		}

		// Continue with the default close behavior
		event.Skip();  // Ensures the window will actually close
	}
}


void AdminFrame::load_users() {
	std::ifstream file("users-schema.json");

	if (!file.is_open()) {
		wxMessageBox("Failed to open users-schema.json", "Error ", wxOK | wxICON_ERROR);
		return;
	}

	json users;
	file >> users;

	for (const auto& user : users) {
		auto new_user = user.get<User>();
		if (new_user != currentUser) {
			usersMap[new_user.get_username()] = std::move(new_user);
		}
	}

	file.close();
}

void AdminFrame::createUser(const std::string& username, const std::string& role, const std::string& password) {
	// Check to see if username already exists
	if (usersMap.find(username) != usersMap.end()) {
		wxMessageBox("User with this username already exists", "Error", wxOK | wxICON_ERROR);
		return;
	}

	if (role != "student" && role != "instructor" && role != "admin") {
		wxMessageBox("This role is not valid", "Error", wxOK | wxICON_ERROR);
		return;
	}

	usersMap[username] = User(username, password, role, {});

	wxUserArray.Add(username);
	userList->Set(wxUserArray);

	wxMessageBox("User created successfully!", "Success", wxOK | wxICON_INFORMATION);
}

void AdminFrame::deleteUser(const std::string& username) {

	if (usersMap.find(username) == usersMap.end()) {
		wxMessageBox("User with this username doesn't exists", "Error", wxOK | wxICON_ERROR);
		return;
	}

	usersMap.erase(username);
	wxUserArray.Remove(username);
	userList->Set(wxUserArray);

	wxMessageBox("User deleted successfully!", "Success", wxOK | wxICON_INFORMATION);
}

void AdminFrame::onCreateUser(wxCommandEvent& event) {
	wxArrayString choices;
	choices.Add("student");
	choices.Add("instructor");
	choices.Add("admin");

	wxTextEntryDialog usernameDlg(this, "Enter username:", "Create User");
	if (usernameDlg.ShowModal() == wxID_OK) {
		std::string username = usernameDlg.GetValue().ToStdString();

		wxSingleChoiceDialog roleDlg(this, "Select role:", "Create User", choices);

		if (roleDlg.ShowModal() == wxID_OK) {
			std::string role = roleDlg.GetStringSelection().ToStdString();

			wxTextEntryDialog passwordDlg(this, "Enter password:", "Create User");
			if (passwordDlg.ShowModal() == wxID_OK) {
				std::string password = passwordDlg.GetValue().ToStdString();
				createUser(username, role, password);
			}
		}
	}
}

void AdminFrame::onDeleteUser(wxCommandEvent& event) {
	wxString selectedUser = userList->GetStringSelection();
	if (selectedUser.IsEmpty()) {
		wxMessageBox("Please select a user to delete.", "Error", wxOK | wxICON_ERROR);
		return;
	}

	wxMessageDialog confirmDlg(this, "Are you sure you want to delete the user '" + selectedUser + "'?", "Confirm Deletion", wxYES_NO | wxNO_DEFAULT | wxICON_WARNING);
	if (confirmDlg.ShowModal() == wxID_YES) {
		deleteUser(selectedUser.ToStdString());
	}
}