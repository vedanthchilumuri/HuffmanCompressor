#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>

struct HuffNode {
    char ch;
    int freq;
    HuffNode *left, *right;
    HuffNode(char c, int f, HuffNode* l = nullptr, HuffNode* r = nullptr)
        : ch(c), freq(f), left(l), right(r) {}
};

struct Compare {
    bool operator()(HuffNode* a, HuffNode* b) {
        return a->freq > b->freq;
    }
};

void buildCodes(HuffNode* node, std::string code, std::unordered_map<char, std::string>& codes) {
    if (!node) return;
    if (!node->left && !node->right) {
        codes[node->ch] = code.empty() ? "0" : code;
        return;
    }
    buildCodes(node->left, code + "0", codes);
    buildCodes(node->right, code + "1", codes);
}

HuffNode* buildTree(const std::string& text) {
    std::unordered_map<char, int> freq;
    for (char c : text) freq[c]++;

    std::priority_queue<HuffNode*, std::vector<HuffNode*>, Compare> pq;
    for (auto& [ch, f] : freq)
        pq.push(new HuffNode(ch, f));

    if (pq.size() == 1) {
        HuffNode* only = pq.top(); pq.pop();
        return new HuffNode('\0', only->freq, only, nullptr);
    }

    while (pq.size() > 1) {
        HuffNode* left = pq.top(); pq.pop();
        HuffNode* right = pq.top(); pq.pop();
        pq.push(new HuffNode('\0', left->freq + right->freq, left, right));
    }

    return pq.top();
}

std::string encode(const std::string& text, std::unordered_map<char, std::string>& codes) {
    std::string out;
    for (char c : text) out += codes[c];
    return out;
}

std::string decode(const std::string& bits, HuffNode* root) {
    std::string out;
    HuffNode* node = root;
    for (char b : bits) {
        node = (b == '0') ? node->left : node->right;
        if (!node->left && !node->right) {
            out += node->ch;
            node = root;
        }
    }
    return out;
}

int main() {
    std::string text =
        "the quick brown fox jumps over the lazy dog "
        "the fox runs fast and the dog barks loudly";

    HuffNode* root = buildTree(text);

    std::unordered_map<char, std::string> codes;
    buildCodes(root, "", codes);

    std::cout << "Huffman codes:\n";
    for (auto& [ch, code] : codes) {
        if (ch == ' ') std::cout << "  'space' -> " << code << "\n";
        else std::cout << "  '" << ch << "' -> " << code << "\n";
    }

    std::string encoded = encode(text, codes);
    std::string decoded = decode(encoded, root);

    long originalBits = (long)text.size() * 8;
    long compressedBits = (long)encoded.size();

    std::cout << "\noriginal size:   " << originalBits << " bits (" << text.size() << " bytes)\n";
    std::cout << "compressed size: " << compressedBits << " bits (" << (compressedBits + 7) / 8 << " bytes)\n";
    std::cout << "compression:     " << (100.0 * compressedBits / originalBits) << "% of original size\n";
    std::cout << "decoded matches original: " << (decoded == text ? "YES" : "NO") << "\n";

    return 0;
}
