#include <iostream>
#include <thread>
#include <vector>
using namespace std;
/*
 * Santa repeatedly sleeps until wakened by either all of his nine reindeer,
 * back from their holidays, or by a group of three of his ten elves.
 * If awakened by the reindeer, he harnesses each of them to his sleigh,
 * delivers toys with them and finally unharnesses them (allowing them
 * to go off on holiday). If awakened by a group of elves, he shows each
 * of the group into his study, consults with them on toy R&D and
 * finally shows them each out (allowing them to go back to work).
 * Santa should give priority to the reindeer in the case that there is
 * both a group of elves and a group of reindeer waiting.
*/
mutex m1;
mutex m2;
mutex m3;
// counting_semaphore<3> three_elves_group(3);
// counting_semaphore<9> all_reindeer_awake(9);
bool reindeer_state[9];
int reindeer_count;
bool elf_state[10];
int elf_count;
void init() {
    reindeer_count = 0;
    for (bool & i : reindeer_state) {
        i = false;
    }
    elf_count = 0;
    for (bool & i : elf_state) {
        i = false;
    }
}
void reindeer_awake(int i) {
    m1.lock();
    if (!reindeer_state[i]) {
        cout << "I am awakened by " << this_thread::get_id() << endl;
        reindeer_state[i] = true;
        reindeer_count++;
    } else {
        cout << "This reindeer is back from their holiday! " << endl;
    }
    m1.unlock();
}
void reindeer_sleep(int i) {
    m1.lock();
    if (reindeer_state[i]) {
        cout << "I am putting to sleep by " << this_thread::get_id() << endl;
        reindeer_state[i] = false;
        reindeer_count--;
    } else {
        cout << "This reindeer is back to their holiday! " << endl;
    }
    m1.unlock();
}
void reindeer_task(int i) {
    reindeer_awake(i);
    reindeer_sleep(i);
}
void elf_awake(int i) {
    m2.lock();
    if (!elf_state[i]) {
        if (elf_count < 3) {
            // i can acquire
            elf_state[i] = true;
            cout << "I need help, Santa, " << i << endl;
            elf_count++;
        }
        else {
            // i cant acquire
            cout << "there are already 3 elves that needs help" << endl;
            m2.unlock();
            return;
        }
    }
    else {
        cout << "This elf already needs help " << i << endl;
    }
    m2.unlock();
}
void elf_sleep(int i) {
    m2.lock();
    if (elf_state[i]) {
        elf_count--;
        elf_state[i] = false;
        cout << "See you later, Santa, " << i << endl;
    }
    else {
        cout << "I don't need help right now, Santa, " << i << endl;
    }
    m2.unlock();
}
void elf_task(int i) {
    elf_awake(i);
    elf_sleep(i);
}
void santa_task(){
        m3.lock();
        if (reindeer_count == 9) { // as reindeer have higher priority
            // all reindeer are awake
            cout<< "I am Santa, It's time to deliver gifts, " << endl;
            for (int i = 0; i < 9; i++) {
                reindeer_sleep(i);
            }
        }
        if (elf_count == 3) {
            cout<< "Helping elves with their toys now! " << endl;
            for (int i = 0; i < 10; i++) {
                if (elf_state[i]) {
                    elf_sleep(i);
                }
            }
        }
        m3.unlock();
}
int main() {
    init();
    vector<thread> reindeer;
    for (int i = 0; i < 9; i++) {
        reindeer.emplace_back(reindeer_awake, i);
    }

    for (int i = 0; i < 9; i++) {
        if (reindeer[i].joinable()) {
            reindeer[i].join();
        }
    }
    elf_awake(0);elf_awake(5);elf_awake(2);elf_awake(6);
    thread santa(santa_task);
    santa.join();
    return 0;
}