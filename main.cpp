#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include <iterator>
#include <memory>

class solution {
public:
    std::vector<std::string> findAllConcatWords(const std::vector<std::string> & words) {
	std::vector<std::string> res;
	std::unique_ptr<TrieNode> root(buildTrie(words));
	
	for (const auto & word : words) {
	    std::stack<size_t> st;
	    st.push(0);
	    std::vector<std::string> path;
	    DFS(word, st, root.get(), path);
            if (!path.empty()) {
                std::cout << "curr = " << word << std::endl;
                std::copy(path.begin(), path.end(), std::ostream_iterator<std::string>(std::cout, " "));
                std::cout << std::endl;
            }
	    if (!path.empty())
		res.push_back(word);
	}
	return res;
    }

private:
    struct TrieNode {
	bool isEnd;
        std::vector<std::unique_ptr<TrieNode>> next;

	TrieNode(bool e=false) : isEnd(e) {
            for (int i = 0; i < 26; i++)
                next.push_back(nullptr);
	}

        ~TrieNode() {
            
        }
    };

    TrieNode * buildTrie(const std::vector<std::string> & strs) {
	TrieNode * root = new TrieNode();
	for (const auto & str : strs) {
	    TrieNode * curr = root;
	    for (size_t i = 0; i < str.size(); i++) {
		if (curr->next[str[i]-'a'] == nullptr) {
		    curr->next[str[i]-'a'].reset(new TrieNode());
		    if (i == str.size()-1)
			curr->next[str[i]-'a']->isEnd = true;
		}
		curr = curr->next[str[i]-'a'].get();
	    }
	}
	return root;
    }

    void DFS(const std::string & s, std::stack<size_t> & st, TrieNode * root, std::vector<std::string> & path) {
	TrieNode * node;
        if (!st.empty()) {
	    size_t curr = st.top();
	    st.pop();
	    if (curr == s.size())
		return;
	    size_t idx = curr;
	    node = root;
	    while (node != nullptr) {
		node = node->next[s[idx]-'a'].get();
		if (node != nullptr && node->isEnd) {
                    if (curr != 0 && idx == s.size()-1) {
                        path.push_back(s.substr(curr, idx+1-curr));
                    }
                    else if (curr == 0 && idx == s.size()-1)
                             st.push(idx+1);
                    else {
                        st.push(idx+1);
                        DFS(s, st, root, path);
                        if (!path.empty())
                            path.insert(path.begin(), s.substr(curr, idx+1-curr));
                    }
		}
		idx++;
	    }
	}
    }
};

int main() {
    std::vector<std::string> words{
	"cat", "cats", "catsdogcats", "dog", "dogcatsdog", "hippopotamuses", "rat", "ratcatdogcat"
    };
    solution soln;
    auto concat = soln.findAllConcatWords(words);
    std::cout << "All the concatenated words are:" << std::endl;
    std::copy(concat.begin(), concat.end(), std::ostream_iterator<std::string>(std::cout, " "));
    std::cout << std::endl;
}
