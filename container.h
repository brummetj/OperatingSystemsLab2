#include <assert.h>

struct node {
    process data;
    node *next;
};

class list {
private:
    node *head, *tail;
public:

    list() {
        head = NULL;
        tail = NULL;
    }

    void createnode(process value) {
        node *temp = new node;
        temp->data = value;
        temp->next = NULL;
        if (head == NULL) {
            head = temp;
            tail = temp;
            temp = NULL;
        } else {
            tail->next = temp;
            tail = temp;
        }
    }

    void display_all() {
        node *temp = new node;
        temp = head;
        while (temp != NULL) {
            std::cout << temp->data.get_pid() << "\t";
            std::cout << temp->data.get_burst() << "\t";
            std::cout << temp->data.get_arrival() << "\t";
            std::cout << temp->data.get_priority() << "\t";
            std::cout << std::endl;
            temp = temp->next;
        }
    }

    void insert_start(process value) {
        node *temp = new node;
        temp->data = value;
        temp->next = head;
        head = temp;
    }

    void insert_position(int pos, process value) {
        node *pre = new node;
        node *cur = new node;
        node *temp = new node;
        cur = head;
        for (int i = 1; i < pos; i++) {
            pre = cur;
            cur = cur->next;
        }
        temp->data = value;
        pre->next = temp;
        temp->next = cur;
    }

    void delete_first() {
        node *temp = new node;
        temp = head;
        head = head->next;
        delete temp;
    }

    void delete_last() {
        node *current = new node;
        node *previous = new node;
        current = head;
        while (current->next != NULL) {
            previous = current;
            current = current->next;
        }
        tail = previous;
        previous->next = NULL;
        delete current;
    }

    void delete_position(int pos) {
        node *current = new node;
        node *previous = new node;
        current = head;
        for (int i = 1; i < pos; i++) {
            previous = current;
            current = current->next;
        }
        previous->next = current->next;
    }

    int size() {
        int count = 0;
        node *current = new node;
        current = head;
        while(current != NULL){
            count++;
            current = current->next;
        }
        return count;
    }
    process index(int index)
    {
        struct node* current = head;
        int count = 0;
        while (current != NULL)
        {
            if (count == index)
                return(current->data);
            count++;
            current = current->next;
        }

        assert(false);
    }
    void insert_end(process value) {

        node* temp = new node;
        temp->data = value;
        temp->next = NULL;

        if(!head) {
            head = temp;
            return;
        } else {
            node* last = head;
            while(last->next) last=last->next;
            last->next = temp;
        }
    }
    void edit(int index, process data) {

        if (index==0) {
            head->data = data;
            return;
        }

        node *pre = head;
        node *temp = NULL;
        for(int i=1; i <= index; i++) {
            temp = pre;
            pre = pre->next;
        }

        node *newNode = new node;
        newNode->data = data;
        temp->next = newNode;
        newNode->next = pre->next;
    }
    void sort_burst(){

        node* curr = NULL;
        node* next = NULL;
        bool unsorted = true;
        while(unsorted){
            unsorted = false;
            curr = head;
            while(curr != NULL){
                 next = curr->next;
                if(next->data.get_burst() < curr->data.get_burst()){
                    curr->next = next->next;
                    next->next = next;
                    unsorted = true;
                }
                curr = curr->next;
            }
        }
    }
};