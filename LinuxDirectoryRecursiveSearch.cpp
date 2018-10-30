#include < stdlib.h >
#include < sys/stat.h >
#include < sys/types.h >
#include < stdint.h >
#include < string >
#include < stdio.h >
#include < errno.h >
#include < iostream >
#include < string.h >
#include < dirent.h >
#include < cstring >
#include < unistd.h >
#include < fstream >

using namespace std;

int overallBinSize;
//z = iterator
int z = 1;


//class declaration
class List {
public:
    class Node {
    public:
        int name, index, binSizeMin, binSizeMax, counter;
        Node * next;
        Node() {
            counter = 0;
            next = NULL;
        }
    };
    List();
    ~List();
    Node * head;
    void insert(int binName);
    bool search(int binName);
    void print();
};

//class function definitions

//constructor
List::List() {
    head = new Node();
}


//destructor
List::~List() {
    Node * temp = new Node();

    while (head -> next != NULL) {
        temp = head -> next;
        head -> next = temp -> next;
        delete temp;
    }
}


//insert function definition
void List::insert(int binName) {

    Node * previous;
    Node * current;
    Node * newNode = new Node();
    newNode -> name = binName;

//check if head == NULL
    if (head -> next == NULL) {
        newNode -> index = 1;
        head -> next = newNode;
        newNode -> counter++;
        return;
    }


    current = head;
    previous = head;

//search for bin
    if (search(binName)) {
        return;
    }

    while (current -> next != NULL) {
        current = current -> next;
        if (binName > current -> name && current -> next == NULL) {
            newNode -> index = current -> index + 1;
            newNode -> next = current -> next;
            current -> next = new Node();
            current -> next = newNode;
        } else if (binName > current -> name && binName > current -> next -> name) {
            previous = current;
            continue;
        } else if (binName < current -> name && current -> next == NULL) {
            newNode -> index = current -> index;
            current -> index += 1;
            newNode -> next = current;
            head -> next = new node;
            head -> next = newNode;
        } else if (binName < current -> name) {
            newNode -> index = current -> index;
            current -> index += 1;
            newNode -> next = current;
            previous -> next = new Node();
            previous -> next = newNode;
            while (current -> next != NULL) {
                current = current -> next;
                current -> index += 1;
            }
        }
        return;
    }
}



//search function definition
bool List::search(int binName) {

    Node * current;
    current = head;

    while (current -> next != NULL) {
        current = current -> next;
        if (binName == current -> name) {
            current -> counter++;
            return 1;
        }
    }
    return 0;
}


//print histogram
void List::print() {

    Node * temp = head;
    while (temp -> next != NULL) {
        temp = temp -> next;
        temp -> binSizeMin = (temp -> index - 1) * overallBinSize;
        temp -> binSizeMax = (temp -> index * overallBinSize) - 1;
        cout << "Bin #: " << temp -> name << endl << "# of files: " << temp -> counter << endl << "temp->binSizeMin: " << temp -> binSizeMin << endl << "temp->binSizeMax: " << temp -> binSizeMax << endl << endl;
    }
}


//recursive function
void recursive(DIR * directory, struct dirent * directP,
               const char * pathname, List test) {

    bool isDirectory = false;

    if ((directory = opendir(pathname)) == NULL) {
        cout << "Could not open directory: " << pathname << endl;
    }

    while ((directP = readdir(directory)) != NULL) {
        cout << "Iteration: " << z << endl << endl;
        if ((directP -> d_type) == DT_UNKNOWN) {
            cout << "Unknown" << endl << endl;
            struct stat buf;
            stat(directP -> d_name, & buf);
            isDirectory = S_ISDIR(buf.st_mode);
        } else if ((directP -> d_type) == DT_REG) {
            cout << "Insert " << directP -> d_name << endl << "File Size: " << directP -> d_reclen << endl << endl;
            test.insert(directP -> d_reclen);
            z++;
            continue;
        } else if ((directP -> d_type) == DT_DIR) {
            cout << "Type= DIR name:" << directP -> d_name << ":" << endl << endl;
        }
        if ((strcasecmp(directP -> d_name, ".") == 0) || (strcasecmp(directP -> d_name, "..") == 0)) {
            cout << "Found dot" << endl << endl;
            z++;
            continue;
        }
        string newPathway = string(pathname) + "/" + directP -> d_name;
        z++;
        recursive(directory, directP, newPathway.c_str(), test);
        return;
    }

    cout << "done" << endl << endl;
    test.print();
    return;
}


//main
int main(int argc, char * argv[]) {
    if (argc != 3) {
        if (argv[1] == NULL) {
            cout << "No directory inputed" << endl;
            return 1;
        } else if (argv[2] == NULL) {
            cout << "No size inputted" << endl;
            return 2;
        } else {
            cout << "Invalid " << endl;
            return 3;
        }
    }

    List test;
    overallBinSize = atoi(argv[2]);
    DIR * cwd;
    struct dirent * cwdP;

    if ((cwd = opendir(argv[1])) == NULL) {
        cout << "Cannot open main directory";
        return 2;
    }

    recursive(cwd, cwdP, argv[1], test);

    while ((closedir(cwd) == -1) && (errno == EINTR));
    {}

    return 0;
}