## Compiling the Project

### For VS Code Users

1.  Make sure you have a C++ compiler like g++ installed and configured.
2.  You can use the following command to compile the project. Replace `<path-to-g++>` with the actual path to your g++ executable and `<project-directory>` with the absolute path to the project directory.

    ```bash
    <path-to-g++>/g++.exe -Wall -Wextra -g3 <project-directory>/XO_Demo.cpp <project-directory>/XO_Classes.cpp -o <project-directory>/output/XO_Demo.exe
    ```

### Using VS Code Tasks

Alternatively, you can create a `tasks.json` file in your `.vscode` directory with the following content. This will allow you to build the project using the `Terminal > Run Build Task` command in VS Code.

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "<path-to-g++>/g++.exe",
            "args": [
                "-Wall",
                "-Wextra",
                "-g3",
                "${workspaceFolder}/XO_Demo.cpp",
                "${workspaceFolder}/XO_Classes.cpp",
                "-o",
                "${workspaceFolder}/output/XO_Demo.exe"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": [
                "$gcc"
            ],
            "detail": "compiler: <path-to-g++>/g++.exe"
        }
    ]
}
```
Make sure to replace `<path-to-g++>` with the path to your g++ executable in the `command` and `detail` fields.
