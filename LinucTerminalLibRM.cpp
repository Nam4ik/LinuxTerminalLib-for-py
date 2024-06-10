// terminal.cpp
#include <Python.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

// Функция для запуска команды в терминале и получения ее вывода
std::string run_command(const char* command) {
    int pipefd[2];
    pipe(pipefd);

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        execlp("/bin/sh", "sh", "-c", command, NULL);
    } else {
        // Parent process
        close(pipefd[1]);
        char buffer[1024];
        ssize_t bytes_read;
        std::string output;

        while ((bytes_read = read(pipefd[0], buffer, 1024)) > 0) {
            output += std::string(buffer, bytes_read);
        }

        waitpid(pid, NULL, 0);
        return output;
    }
}

// Функция для установки терминального режима
void set_terminal_mode(int fd, int mode) {
    struct termios t;
    tcgetattr(fd, &t);
    if (mode == 1) {
        // Raw mode
        t.c_lflag &= ~(ICANON | ECHO);
        t.c_cc[VMIN] = 1;
        t.c_cc[VTIME] = 0;
    } else {
        // Cooked mode
        t.c_lflag |= ICANON | ECHO;
    }
    tcsetattr(fd, TCSANOW, &t);
}

// Функция для чтения из терминала
std::string read_from_terminal(int fd) {
    char buffer[1024];
    ssize_t bytes_read;
    std::string input;

    while ((bytes_read = read(fd, buffer, 1024)) > 0) {
        input += std::string(buffer, bytes_read);
    }

    return input;
}

// Функция для записи в терминал
void write_to_terminal(int fd, const char* data) {
    write(fd, data, strlen(data));
}

// Python wrapper functions
static PyObject* py_run_command(PyObject* self, PyObject* args) {
    const char* command;
    if (!PyArg_ParseTuple(args, "s", &command)) {
        return NULL;
    }

    std::string output = run_command(command);
    return Py_BuildValue("s", output.c_str());
}

static PyObject* py_set_terminal_mode(PyObject* self, PyObject* args) {
    int fd, mode;
    if (!PyArg_ParseTuple(args, "ii", &fd, &mode)) {
        return NULL;
    }

    set_terminal_mode(fd, mode);
    Py_RETURN_NONE;
}

static PyObject* py_read_from_terminal(PyObject* self, PyObject* args) {
    int fd;
    if (!PyArg_ParseTuple(args, "i", &fd)) {
        return NULL;
    }

    std::string input = read_from_terminal(fd);
    return Py_BuildValue("s", input.c_str());
}

static PyObject* py_write_to_terminal(PyObject* self, PyObject* args) {
    int fd;
    const char* data;
    if (!PyArg_ParseTuple(args, "is", &fd, &data)) {
        return NULL;
    }

    write_to_terminal(fd, data);
    Py_RETURN_NONE;
}

static PyMethodDef TerminalMethods[] = {
    {"run_command", py_run_command, METH_VARARGS, "Run a command in the terminal"},
    {"set_terminal_mode", py_set_terminal_mode, METH_VARARGS, "Set the terminal mode"},
    {"read_from_terminal", py_read_from_terminal, METH_VARARGS, "Read from the terminal"},
    {"write_to_terminal", py_write_to_terminal, METH_VARARGS, "Write to the terminal"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef terminal_module = {
    PyModuleDef_HEAD_INIT,
    "terminal",
    "Terminal module",
    -1,
    TerminalMethods
};

PyMODINIT_FUNC PyInit_terminal(void) {
    return PyModule_Create(&terminal_module);
}
