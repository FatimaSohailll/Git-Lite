#include "Key.h"

    Key:: Key() :value("-1") {}
    Key::Key(int value) :value(value) {}

    Key::Key(const Key& copy) {
        duplicates.clearList();
        this->value = copy.value;
        ListNode<Record>* otherN = copy.duplicates.head;
        while (otherN) {  //deep copy
            this->duplicates.insert(otherN->data);
            otherN = otherN->next;
        }
    }

    void Key::operator=(const Key& other) {
        duplicates.clearList();
        this->value = other.value;
        ListNode<Record>* otherN = other.duplicates.head;
        while (otherN) {  //deep copy
            this->duplicates.insert(otherN->data);
            otherN = otherN->next;
        }
    }
    void Key::clearKey() {
        value = "-1";
        duplicates.clearList();
    }
    void Key::insertDuplicates(str record) {
        Record newRecord;
        newRecord.insertRecord(record);
        duplicates.insert(newRecord);
    }
    bool Key::operator==(const Key& other) {
        return (this->value == other.value);
    }
    Key::~Key() {
        if (this->duplicates.head != nullptr)
            this->duplicates.clearList();
    }

ostream& operator<<(ostream& output, Key& key) {
    cout << key.value;
    return output;
}
