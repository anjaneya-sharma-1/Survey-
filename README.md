# Survey++

A C++ Survey Application for creating, managing, and filling out surveys with user authentication and a feedback system.

## Features

### Admin Features

- Create new surveys with multiple question types:
  - Multiple choice questions (MCQ)
  - Open-ended text questions
- View existing surveys
- Read user responses
- View detailed statistics and analytics
- Manage survey questions:
  - Add new questions
  - Edit existing questions
  - Delete questions
- View feedback ratings with visual representation

### Client Features

- User authentication system:
  - Login
  - Signup
- View available surveys
- Fill out surveys
- Provide feedback ratings (1-5 scale)

## Technical Details

### Dependencies

- C++ Standard Library
- Windows API (for screen clearing and colors)
- File I/O for data persistence

### File Structure

```
├── main.cpp         # Entry point and UI flows
├── survey.h         # Survey class declarations
├── survey.cpp       # Survey implementation
├── user.h           # User authentication declarations
├── user.cpp         # User authentication implementation
├── ui.h             # UI utilities declarations
├── ui.cpp           # UI utilities implementation
├── question.h       # Question structure definition
```

### Data Storage

- Survey data stored in text files (`*.txt`)
- User responses stored with `2` prefix
- Feedback ratings stored with `f` prefix
- Client credentials stored in `clients.txt`
- Survey list maintained in `survey_list.txt`

## Usage

### Admin Access

### Building

Compile using a C++ compiler that supports C++11 or later:

```sh
 g++ main.cpp survey.cpp user.cpp ui.cpp -o survey_app
```

### Running

Execute the compiled binary:

```sh
 ./survey_app
```

### System Requirements

- Windows operating system
- C++11 compatible compiler
- Terminal/Console with Unicode support

##
