#include <bits/stdc++.h>
using namespace std;

namespace DoubleLinkList {

struct NODE {
    int data;
    NODE *next;
    NODE *pre;
    NODE() {}
    NODE(int val) { this->data = val; }
};

NODE *head = nullptr, *tail = nullptr;
int len = 0;

void Pre() {
    head = new NODE();
    tail = new NODE();
    head->pre = NULL;
    tail->next = NULL;
    head->next = tail;
    tail->pre = head;
    len = 0;
}

NODE *move(int i) {
    // return pointer to node at position i (0‑based) in the real list
    NODE *cur = head;
    for (int j = 0; j <= i; j++) {
        cur = cur->next;
    }
    return cur;
}

void insert(int i, int x) {
    // insert before position i (i may equal len, meaning before tail)
    NODE *cur = move(i); // node currently at i (or tail if i==len)
    NODE *node = new NODE(x);
    node->next = cur;
    node->pre = cur->pre;
    cur->pre->next = node;
    cur->pre = node;
    ++len;
}

void remove(int i) {
    NODE *cur = move(i);
    cur->pre->next = cur->next;
    cur->next->pre = cur->pre;
    delete cur;
    --len;
}

int Get_length() {
    return len;
}

int Query(int i) {
    if (i < 0 || i >= len) return -1;
    return move(i)->data;
}

void Print() {
    if (len == 0) {
        cout << -1 << '\n';
        return;
    }
    NODE *cur = head->next;
    bool first = true;
    while (cur != tail) {
        if (!first) cout << ' ';
        first = false;
        cout << cur->data;
        cur = cur->next;
    }
    cout << '\n';
}

void Clear() {
    NODE *cur = head->next;
    while (cur != tail) {
        NODE *tmp = cur;
        cur = cur->next;
        delete tmp;
    }
    head->next = tail;
    tail->pre = head;
    len = 0;
}

} // namespace DoubleLinkList

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if(!(cin>>n)) return 0;
    int op, x, p, ans;
    DoubleLinkList::Pre();
    for (int _ = 0; _ < n; ++_) {
        cin >> op;
        switch (op) {
        case 0:
            ans = DoubleLinkList::Get_length();
            cout << ans << '\n';
            break;
        case 1:
            cin >> p >> x;
            DoubleLinkList::insert(p, x);
            break;
        case 2:
            cin >> p;
            ans = DoubleLinkList::Query(p);
            cout << ans << '\n';
            break;
        case 3:
            cin >> p;
            DoubleLinkList::remove(p);
            break;
        case 4:
            DoubleLinkList::Print();
            break;
        }
    }
    DoubleLinkList::Clear();
    return 0;
}

