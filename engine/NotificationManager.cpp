#include "NotificationManager.h"

#include "GameObject.h"
#include <algorithm>

namespace CMPUT350 {

//Register: skeleton | AI did: explained weak_ptr listeners, gave skeleton with blanks | I did: filled the blanks
void NotificationManager::Register(std::weak_ptr<GameObject> object, const std::string& key) {
    mListeners[key].push_back(object);
}

//Unregister: skeleton | AI did: gave remove_if skeleton with two blanks, then told you the lambda condition and the vec name fix | I did: wrote the lookup, typed the code
void NotificationManager::Unregister(std::weak_ptr<GameObject> object, const std::string& key){
    auto found = mListeners.find(key);
    if (found == mListeners.end()) {return;}

    auto target = object.lock();
    auto &vec = found->second;
    vec.erase(std::remove_if(vec.begin(), vec.end(),
        [&](const std::weak_ptr<GameObject> &w) {
            auto p = w.lock();
            return !p || p == target;   // remove it if it's expired OR it's the object we want gone
        }), vec.end());
}

//Notify: skeleton | AI did: explained copy-before-iterate, pointed out the obj-> bug, gave the cleanup skeleton and its two answers (message, expired) | I did: wrote the find/copy/loop
void NotificationManager::Notify(const std::string &message) {
    auto it = mListeners.find(message);
    if (it == mListeners.end()) return;

    auto listeners = it->second;
    for (auto &weak : listeners) {
        if (auto obj = weak.lock()) {
            obj->ReceiveNotification(message);
        }
    }
    auto &vec = mListeners[message];   // look the list up again by key, don't reuse `it`
    vec.erase(std::remove_if(vec.begin(), vec.end(),[](const std::weak_ptr<GameObject> &w) { return w.expired(); }), vec.end());
}

}