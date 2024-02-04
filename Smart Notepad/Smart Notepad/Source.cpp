#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Node 
{
    char character;
    Node* left;
    Node* right;
    Node* top;
    Node* bottom;
    int rightCount = 0;
    int bottomCount = 0;

    Node(char ch) : character(ch), left(nullptr), right(nullptr), top(nullptr), bottom(nullptr) {}
};

class AdvancedNotepad 
{
private:

    Node* root;

public:
    AdvancedNotepad() : root(nullptr) {}

    void addText(int x, int y, const string& text) 
    {
        if (x < 0 || y < 0) 
        {
            cout << "Invalid position." << endl;
            return;
        }

        if (root == nullptr) 
        {
            root = new Node('\0');
        }

        Node* currentNode = root;

        for (int i = 0; i < y; i++)
        {
            if (currentNode->bottom == nullptr) 
            {
                currentNode->bottom = new Node('\0');
                currentNode->bottom->top = currentNode;
            }

            currentNode = currentNode->bottom;
        }

        for (int i = 0; i < x; i++) 
        {
            if (currentNode->right == nullptr) 
            {
                currentNode->right = new Node('\0');
                currentNode->right->left = currentNode;
            }

            currentNode = currentNode->right;
        }

        for (char ch : text)
        {
            Node* newNode = new Node(ch);

            newNode->left = currentNode;
            newNode->right = currentNode->right;
            newNode->top = currentNode->top;
            newNode->bottom = currentNode->bottom;

            if (currentNode->right != nullptr)
                currentNode->right->left = newNode;
            currentNode->right = newNode;

            currentNode = newNode;
        }
    }

    void deleteText(const string& text) 
    {
        Node* currentNode = root;

        while (currentNode != nullptr) 
        {
            Node* nextNode = currentNode;

            while (nextNode != nullptr && nextNode->character == text[0]) 
            {
                nextNode = nextNode->right;
            }

            Node* matchNode = findTextMatch(nextNode, text);

            if (matchNode != nullptr) 
            {
                deleteMatchedText(matchNode, text);
                currentNode = matchNode;
            }
            else 
            {
                currentNode = nextNode;
            }
        }
    }

    void searchText(const string& text) 
    {
        Node* currentNode = root;
        int position = 0;
        int matchCount = 0;

        while (currentNode != nullptr)
        {
            if (currentNode->character == text[position]) 
            {
                position++;

                if (position == text.length()) 
                {
                    cout << "Match found at position (" << currentNode->rightCount - text.length() + 1 << ", " << currentNode->bottomCount + 1 << ")" << endl;
                    matchCount++;
                }
            }
            else 
            {
                position = 0;
            }

            currentNode = currentNode->right;
        }

        if (matchCount == 0)
            cout << "No matches found." << endl;
    }

    void copyText(int startX, int startY, int endX, int endY, string& clipboard) 
    {
        if (startX < 0 || startY < 0 || endX < 0 || endY < 0) 
        {
            cout << "Invalid position." << endl;
            return;
        }

        if (startY > endY || (startY == endY && startX > endX)) 
        {
            cout << "Invalid range." << endl;
            return;
        }

        Node* startNode = getNodeAtPosition(startX, startY);
        Node* endNode = getNodeAtPosition(endX, endY);

        if (startNode == nullptr || endNode == nullptr) 
        {
            cout << "Invalid position." << endl;
            return;
        }

        clipboard.clear();

        Node* currentNode = startNode;

        while (currentNode != nullptr) 
        {
            clipboard += currentNode->character;

            if (currentNode == endNode)
                break;

            if (currentNode->right == nullptr)
                currentNode = currentNode->bottom;
            else
                currentNode = currentNode->right;
        }
    }

    void pasteText(int x, int y, const string& clipboard) 
    {
        if (x < 0 || y < 0)
        {
            cout << "Invalid position." << endl;
            return;
        }

        if (root == nullptr) 
        {
            root = new Node('\0');
        }

        Node* currentNode = getNodeAtPosition(x, y);

        if (currentNode == nullptr) 
        {
            cout << "Invalid position." << endl;
            return;
        }

        for (char ch : clipboard) 
        {
            if (currentNode->right == nullptr) 
            {
                Node* newNode = new Node(ch);

                newNode->left = currentNode;
                newNode->top = currentNode->top;
                newNode->bottom = currentNode->bottom;

                if (currentNode->top != nullptr)
                    currentNode->top->bottom = newNode;
                if (currentNode->bottom != nullptr)
                    currentNode->bottom->top = newNode;

                currentNode->right = newNode;

                currentNode = newNode;
            }
            else 
            {
                Node* newNode = new Node(ch);

                newNode->left = currentNode;
                newNode->right = currentNode->right;
                newNode->top = currentNode->top;
                newNode->bottom = currentNode->bottom;

                if (currentNode->right != nullptr)
                    currentNode->right->left = newNode;
                currentNode->right = newNode;

                currentNode = newNode;
            }
        }
    }

    void findAndReplace(const string& word, const string& replacement, bool replaceAll) 
    {
        Node* currentNode = root;
        int position = 0;
        int replaceCount = 0;

        while (currentNode != nullptr)
        {
            if (currentNode->character == word[position]) 
            {
                position++;

                if (position == word.length()) 
                {
                    replaceWord(currentNode, word, replacement);
                    position = 0;
                    replaceCount++;
                    if (!replaceAll)
                        break;
                }
            }
            else 
            {
                position = 0;
            }

            currentNode = currentNode->right;
        }

        if (replaceCount == 0)
            cout << "No occurrences found." << endl;
        else
            cout << "Replaced " << replaceCount << " occurrence(s)." << endl;
    }

    void undo() 
    {
        // TODO: Implement undo functionality using a stack.
        cout << "Undo operation." << endl;
    }

    int countWords()
    {
        int wordCount = 0;

        Node* currentNode = root;

        while (currentNode != nullptr)
        {
            if (isWordStart(currentNode)) 
            {
                wordCount++;
            }

            currentNode = currentNode->right;
        }

        return wordCount;
    }

    void wordSuggestion() 
    {
        // TODO: Implement word suggestion functionality.
        cout << "Word suggestion." << endl;
    }

    void saveText(const string& filename)
    {
        ofstream outputFile(filename);

        if (!outputFile) 
        {
            cout << "Failed to open file: " << filename << endl;
            return;
        }

        Node* currentNode = root;

        while (currentNode != nullptr) 
        {
            Node* rowStart = currentNode;

            while (currentNode != nullptr) 
            {
                outputFile << currentNode->character;
                currentNode = currentNode->right;
            }

            outputFile << endl;

            currentNode = rowStart->bottom;
        }

        outputFile.close();

        cout << "Text saved to file: " << filename << endl;
    }

    void similarityChecker(const string& file1, const string& file2) 
    {
        ifstream file1Stream(file1);
        ifstream file2Stream(file2);

        if (!file1Stream) 
        {
            cout << "Failed to open file: " << file1 << endl;
            return;
        }

        if (!file2Stream) 
        {
            cout << "Failed to open file: " << file2 << endl;
            return;
        }

        string file1Content((istreambuf_iterator<char>(file1Stream)), istreambuf_iterator<char>());
        string file2Content((istreambuf_iterator<char>(file2Stream)), istreambuf_iterator<char>());

        file1Stream.close();
        file2Stream.close();

        // TODO: Implement similarity checking algorithm.

        cout << "Similarity checked." << endl;
    }

private:
    Node* getNodeAtPosition(int x, int y) 
    {
        Node* currentNode = root;

        for (int i = 0; i < y; i++) 
        {
            if (currentNode == nullptr || currentNode->bottom == nullptr)
                return nullptr;

            currentNode = currentNode->bottom;
        }

        for (int i = 0; i < x; i++) 
        {
            if (currentNode == nullptr || currentNode->right == nullptr)
                return nullptr;

            currentNode = currentNode->right;
        }

        return currentNode;
    }

    Node* findTextMatch(Node* startNode, const string& text) 
    {
        while (startNode != nullptr) 
        {
            if (startNode->character == text[0])
                return startNode;

            startNode = startNode->right;
        }

        return nullptr;
    }

    void deleteMatchedText(Node* startNode, const string& text) 
    {
        Node* currentNode = startNode;
        int position = 0;

        while (currentNode != nullptr) 
        {
            if (currentNode->character == text[position]) 
            {
                position++;

                if (position == text.length()) 
                {
                    Node* previousNode = currentNode->left;

                    while (currentNode != nullptr && currentNode->character == text[position - 1]) 
                    {
                        Node* nextNode = currentNode->right;

                        if (previousNode != nullptr)
                            previousNode->right = nextNode;
                        if (nextNode != nullptr)
                            nextNode->left = previousNode;

                        delete currentNode;

                        currentNode = nextNode;
                    }

                    break;
                }
            }
            else 
            {
                break;
            }

            currentNode = currentNode->right;
        }
    }

    bool isWordStart(Node* node) 
    {
        if (node->left == nullptr || node->left->character == ' ')
            return true;

        return false;
    }

    void replaceWord(Node* startNode, const string& word, const string& replacement) 
    {
        Node* currentNode = startNode;
        int position = 0;

        while (currentNode != nullptr) 
        {
            if (currentNode->character == word[position])
            {
                position++;

                if (position == word.length()) 
                {
                    Node* replaceNode = currentNode;
                    Node* previousNode = currentNode->left;

                    while (replaceNode != nullptr && replaceNode->character == word[position - 1])
                    {
                        Node* nextNode = replaceNode->right;

                        if (previousNode != nullptr)
                            previousNode->right = nextNode;
                        if (nextNode != nullptr)
                            nextNode->left = previousNode;

                        delete replaceNode;

                        replaceNode = nextNode;
                    }

                    for (char ch : replacement) 
                    {
                        Node* newNode = new Node(ch);

                        newNode->left = previousNode;
                        newNode->right = previousNode->right;
                        newNode->top = currentNode->top;
                        newNode->bottom = currentNode->bottom;

                        if (previousNode->right != nullptr)
                            previousNode->right->left = newNode;
                        previousNode->right = newNode;

                        previousNode = newNode;
                    }

                    break;
                }
            }
            else 
            {
                break;
            }

            currentNode = currentNode->right;
        }
    }
};

int main() 
{
    AdvancedNotepad notepad;
    string clipboard;

    while (true) 
    {
        int choice;
        cout << "**********Advanced Notepad**********\n" << endl;
        cout << "1. Add Text" << endl;
        cout << "2. Delete Text" << endl;
        cout << "3. Search Words" << endl;
        cout << "4. Copy Text" << endl;
        cout << "5. Paste Text" << endl;
        cout << "6. Find and Replace" << endl;
        cout << "7. Undo" << endl;
        cout << "8. Count Total Words" << endl;
        cout << "9. Word Suggestion" << endl;
        cout << "10. Save Text" << endl;
        cout << "11. Similarity Checker" << endl;
        cout << "12. Exit" << endl;
        cout << "\nEnter your choice: ";
        cin >> choice;


        switch (choice) 
        {
        case 1:
        {
            int x, y;
            string text;

            cout << "Enter x position: ";
            cin >> x;
            cout << "Enter y position: ";
            cin >> y;
            cout << "Enter text to insert: ";
            cin.ignore();
            getline(cin, text);

            notepad.addText(x, y, text);
            break;
        }
        case 2: 
        {
            string text;

            cout << "Enter text to delete: ";
            cin.ignore();
            getline(cin, text);

            notepad.deleteText(text);
            break;
        }
        case 3: 
        {
            string text;

            cout << "Enter text to search: ";
            cin.ignore();
            getline(cin, text);

            notepad.searchText(text);
            break;
        }
        case 4: 
        {
            int startX, startY, endX, endY;

            cout << "Enter start x position: ";
            cin >> startX;
            cout << "Enter start y position: ";
            cin >> startY;
            cout << "Enter end x position: ";
            cin >> endX;
            cout << "Enter end y position: ";
            cin >> endY;

            notepad.copyText(startX, startY, endX, endY, clipboard);
            break;
        }
        case 5: 
        {
            int x, y;

            cout << "Enter x position to paste: ";
            cin >> x;
            cout << "Enter y position to paste: ";
            cin >> y;

            notepad.pasteText(x, y, clipboard);
            break;
        }
        case 6:
        {
            string word, replacement;
            int option;

            cout << "Enter word to find: ";
            cin.ignore();
            getline(cin, word);
            cout << "Enter replacement word: ";
            getline(cin, replacement);
            cout << "Choose an option:" << endl;
            cout << "1. Replace the first occurrence" << endl;
            cout << "2. Replace all occurrences" << endl;
            cout << "Enter option: ";
            cin >> option;

            notepad.findAndReplace(word, replacement, option == 2);
            break;
        }
        case 7: 
        {
            notepad.undo();
            break;
        }
        case 8:
        {
            int wordCount = notepad.countWords();
            cout << "Total words: " << wordCount << endl;
            break;
        }
        case 9: 
        {
            notepad.wordSuggestion();
            break;
        }
        case 10: 
        {
            string filename;

            cout << "Enter file name to save: ";
            cin.ignore();
            getline(cin, filename);

            notepad.saveText(filename);
            break;
        }
        case 11:
        {
            string file1, file2;

            cout << "Enter first file name: ";
            cin.ignore();
            getline(cin, file1);
            cout << "Enter second file name: ";
            getline(cin, file2);

            notepad.similarityChecker(file1, file2);
            break;
        }
        case 12:
        {
            exit(0);
        }
        default:
        {
            cout << "Invalid choice." << endl;
        }
   
        }

        cout << endl;
    }

    system("pause");
    return 0;
}
