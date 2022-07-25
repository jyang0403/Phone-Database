#include <iostream>
#include <sys/types.h>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <cctype>
#include <algorithm>

using std::string;
using std::map;
using std::cout;
using std::cin;
using std::ofstream;
using std::ifstream;
using std::endl;

// Name and Phone store all information about a contact
struct Name {
  string first_name_orig;
  string last_name_orig;
  string first_name_lower;
  string last_name_lower;
};
struct Phone {
  string phone_number[5] = {"EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"};
  int type[5] = {0,0,0,0,0};
  string type_names[5] = {"CELL", "FAX", "HOME", "VOIP", "WORK"};
};

//changing the operator < for Name objects to determine which names come first alphabetically
bool operator<(const Name &left, const Name &right) {
  if (left.last_name_lower < right.last_name_lower) {
    return true;
  }
  else if (left.last_name_lower == right.last_name_lower) {
    return left.first_name_lower < right.first_name_lower;
  }
  else {
    return false;
  }
}
// helper method to convert names into lowercase for comparison
string make_lower(string input) {
  for (int i = 0; i < (int)input.length(); i++) {
    input[i] = tolower(input[i]);
  }
  return input;
}
//creates a Name for a contact given the first and last name in the format inputted by user
Name create_name(string first_name_input, string last_name_input) {
  Name name;
  name.first_name_lower = make_lower(first_name_input);
  name.first_name_orig = first_name_input;
  name.last_name_lower = make_lower(last_name_input);
  name.last_name_orig = last_name_input;
  return name;
}
//checks if a phone number is valid or not
int check_phone(map<Name, Phone> db, string first_name, string last_name, string number, string type) {
  int check = 0;
  int res;
  // check if contact present
  for(map<Name,Phone>::iterator it = db.begin(); it != db.end(); it++) {
    if (it->first.first_name_lower == first_name && it->first.last_name_lower == last_name) {
      check++;
      break;
    }
  }
  if (!check) {
    cout << "Error: Contact not found\n";
    return 0;
  }
  // check if phone type is valid for main
  if (type == "CELL") {
    res = 1;
  }
  else if (type == "FAX") {
    res = 2;
  }
  else if (type == "HOME") {
    res = 3;
  }
  else if (type == "VOIP") {
    res = 4;
  }
  else if (type == "WORK") {
    res = 5;
  }
  else {
    cout << "Error: Invalid phone number type\n";
    return 0;
  }
  //check for valid phone number
  //check the first and last letters to see if they are '-'
  if (number[0] == '-' || number[number.length() - 1] == '-') {
    cout << "Error: Not a valid phone number\n";
    return 0;
  }
  //check everything else
  for (int i = 0; i < (int)number.length(); i++) {
    if (!isdigit(number[i]) && number[i] != '-') {
      cout << "Error: Not a valid phone number\n";
      return 0;
    }
  }
  return res;
}
// adds a contact to the phone database with empty phone
void create_contact(map<Name,Phone>* db, Name name, Phone phone, int* size) {
  for(map<Name,Phone>::iterator it = db->begin();it != db->end(); it++) {
    if (it->first.last_name_lower == name.last_name_lower && it->first.first_name_lower == name.first_name_lower) {
      cout << "Error: Contact already exists\n";
      return;
    }
  }
  db->insert({name, phone});
  if ((int)db->size() == (*size) + 1) {
    cout << "Result: Contact created\n";
    *size += 1;
    return;
  }
  return;
}
// removes a contact from the database
void delete_contact(map<Name,Phone> * db, Name name, int* size) {
  for(map<Name,Phone>::iterator it = db->begin(); it != db->end(); it++) {
    if (it->first.last_name_lower == name.last_name_lower && it->first.first_name_lower == name.first_name_lower) {
      (*db).erase(it->first);
      break;
    }
  }
  // use size to see if deleted successfully, make the size smaller
  if ((int)db->size() == (*size) - 1 && *size > 0) {
    printf("Result: Contact deleted\n");
    *size -= 1;
    return;
  }
  else {
    printf("Error: Contact not found\n");
    return;
  }
  return;
}
// lists the contacts of the database
void list_contacts(map<Name,Phone> db) {
  int count = 0;
  // list and count contacts by iterator
  for(map<Name,Phone>::iterator it = db.begin(); it != db.end(); it++) {
    cout << "Result: " << it->first.last_name_orig << ","<<it->first.first_name_orig << "\n";
    count++;
  }
  if (count > 0) {
    cout << "Info: there are " << count << " contact(s)\n";
  }
  return;
}
// lists phone numbers of a specified contact
void list_phone_numbers(map<Name,Phone> db, string first_name, string last_name) {
  int check = 0;
  int count = 0;
  for(map<Name,Phone>::iterator it = db.begin(); it != db.end(); it++) {
    if (it->first.first_name_lower == first_name && it->first.last_name_lower == last_name) {
      check++;
      // print and count phone numbers by iterator
      for (int i = 0; i < 5; i++) {
        if (it->second.type[i] != 0) {
          cout << "Result: " << it->second.type_names[i] << "," << it->second.phone_number[i] << "\n";
          count++;
        }
      }
      break;
    }
  }
  if (check > 0) {
    cout << "Info: Found " << count << " phone number(s) for this contact\n";
  }
  else {
    cout << "Error: Contact not found\n";
  }
  return;
}
// adds or replaces a contact's phone number 
void add_or_replace(map<Name,Phone> * db, string phone_number, string first_name, string last_name, int type) {
  //decrement to match array indexing
  type--;
  for(map<Name,Phone>::iterator it = db->begin(); it != db->end(); it++) {
    if (it->first.first_name_lower == first_name && it->first.last_name_lower == last_name) {
      // if phone number exists, then replace, else add
      if (it->second.type[type] != 0) {
        cout << "Result: Phone number replaced\n";
      }
      else {
        cout << "Result: Phone number added\n";
      }
      it->second.type[type] = 1;
      it->second.phone_number[type] = phone_number;
      return;
    }
  }
}
// deletes a contact's phone number and makes the slot empty
void delete_phone_num(map<Name,Phone> * db, string first_name, string last_name, string type) {
  int check = 0;
  int phone_index;
  map<Name,Phone>::iterator it = db->begin();
  if (type == "CELL") {
    phone_index = 0;
  }
  else if (type == "FAX") {
    phone_index = 1;
  }
  else if (type == "HOME") {
    phone_index = 2;
  }
  else if (type == "VOIP") {
    phone_index = 3;
  }
  else if (type == "WORK") {
    phone_index = 4;
  }
  else {
    cout << "Error: No phone number of that type\n";
    return;
  }
  // see if contact exists: if so, then replace the number. if not, return error
  for(; it != db->end(); it++) {
    if (it->first.first_name_lower == first_name && it->first.last_name_lower == last_name) {
      check++;
      break;
    }
  }
  if (check == 0) {
    cout << "Error: Contact not found\n";
  }
  else {
    if (it->second.type[phone_index] == 0) {
      cout << "Error: No phone number of that type\n";
      return;
    }
    else {
      it->second.phone_number[phone_index] = "EMPTY";
      it->second.type[phone_index] = 0;
      cout << "Result: Phone number deleted\n";
      return;
    }
  }
  cout << "Result: Phone number deleted\n";
  return;
}
// saves database info to file 
void save_to_file(map<Name,Phone> db, string file_name) {
  ofstream output(file_name);
  if (!output.is_open()) {
    cout << "Error: Could not open output file\n";
    return;
  }
  for(map<Name,Phone>::iterator it = db.begin(); it != db.end(); it++) {
    // clearly establish when a new contact begins
    output << "=NEW=" << endl;
    output << "=NAME_INFO=" << endl;
    output << it->first.first_name_orig << endl;
    output << it->first.last_name_orig << endl;
    output << it->first.first_name_lower << endl;
    output << it->first.last_name_lower << endl;
    output << "=PHONE_INFO=" << endl;
    for (int i = 0; i < 5; i++) {
      output << it->second.type[i] << " ";
    }
    output << endl;
    for (int i = 0; i < 5; i++) {
      output << it->second.phone_number[i] << " ";
    }
    output << endl;
  }
  output << "=END=";
  cout << "Result: Database saved in file " << file_name << "\n";
  return;
}
// retrieves database info from file
map<Name,Phone> restore_from_file(map<Name,Phone> db, int* size, string file_name) {
  ifstream input(file_name);
  if (!input.is_open()) {
    cout << "Error: Could not open input file\n";
    return db;
  }
  map<Name,Phone> new_db;
  string next_string;
  string dump;
  *size = 0;
  while(1) {
    input >> next_string;
    // stop reading
    if (next_string == "=END=") {
      cout << "Result: Database restored from file " << file_name << "\n";
      return new_db;
      break;
    }
    // create new contact
    else if (next_string == "=NEW=") {
      input >> next_string;
      if (next_string != "=NAME_INFO=") {
        cout << "Error: Invalid database file\n";
        return new_db;
      }
      Name name;
      Phone phone;
      input >> name.first_name_orig >> name.last_name_orig >> name.first_name_lower >> name.last_name_lower;
      input >> next_string;
      if (next_string != "=PHONE_INFO=") {
        cout << "Error: Invalid database file\n";
        return new_db;
      }
      for (int i = 0; i < 5; i++) {
        input >> phone.type[i];
      }
      // get phone numbers for existing spots
      for (int i = 0; i < 5; i++) {
        if (phone.type[i] == 1) {
          input >> phone.phone_number[i];
        }
        else {
          input >> dump;
        }
      }
      new_db.insert({name, phone});
      // check to see if key already exists: if it does, invalid file
      if ((int) new_db.size() == *size + 1) {
        *size+=1;
      }
      else {
        cout << "Error: Invalid database file\n";
        return new_db;
      }
    }
    // if each contact is not separated by "new" or if end of file is not "end", or other reading mistake then invalid file
    else {
      cout << "Error: Invalid database file\n";
        return new_db;
    }
  }
}

int main() {
  int size = 0;
  int number_check;
  map<Name, Phone> phone_database;
  char option;
  Phone empty_phone;
  Phone phone;
  Name new_name;
  string first_name;
  string last_name;
  string phone_number;
  string phone_type;
  string file_name;
  cout << "Info: Welcome to the Phone database!\n"; 
  cout << "Info: Please enter a command\n"; 
  while (cin >> option) {
    if (option == 'Q') {
      break; // end program
    }
    switch (option) {
      case 'C': 
        cin >> last_name >> first_name;
        new_name = create_name(first_name, last_name);
        create_contact(&phone_database, new_name, empty_phone, &size);
        break;
      case 'D':
        cin >> last_name >> first_name;
        new_name = create_name(first_name, last_name);
        delete_contact(&phone_database, new_name, &size);
        break;
      case 'L': 
        list_contacts(phone_database);
        break;
      case 'N':
        cin >> last_name >> first_name >> phone_type >> phone_number;
        first_name = make_lower(first_name);
        last_name = make_lower(last_name);
        number_check = check_phone(phone_database, first_name, last_name, phone_number, phone_type);
        if (number_check) {
          add_or_replace(&phone_database, phone_number, first_name, last_name, number_check--);
        }
        break;
      case 'P':
        cin >> last_name >> first_name;
        first_name = make_lower(first_name);
        last_name = make_lower(last_name);
        list_phone_numbers(phone_database, first_name, last_name);
        break;
      case 'X':
        cin >> last_name >> first_name >> phone_type;
        first_name = make_lower(first_name);
        last_name = make_lower(last_name);
        delete_phone_num(&phone_database, first_name, last_name, phone_type);
        break;
      case 'S': 
        cin >> file_name;
        save_to_file(phone_database, file_name);
        break;
      case 'R':
        cin >> file_name;
        phone_database = restore_from_file(phone_database, &size, file_name);
        break;
      default: 
        cout << "Error: invalid command\n";
        // if command is invalid, ignore the rest of the line to prevent repetition of invalid command message
        cin.ignore(1000, '\n');
        break;
    }
    cout << "Info: Please enter a command\n";
  }
  std::cout << "Info: Thank you for using the Phone Database!\n"; 
  return 0;
}
