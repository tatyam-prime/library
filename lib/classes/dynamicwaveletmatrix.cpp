template <typename T, int W>
struct WaveletMatrix{

    array<DynamicBitVector, W> bv;

    WaveletMatrix(vector<T>& a){
        int n = a.size();
        vector<T> v(a);
        for(int i = W - 1; i >= 0; --i){
            vector<uint64_t> b((n + 31) >> 5, 0);
            vector<int> length(b.size(), 0);
            vector<T> v1, v2;
            for(int j = 0; j < n; ++j){
                bool fl = ((v[j] >> i) & 1);
                (fl ? v2 : v1).push_back(v[j]);
                b[j >> 5] |= uint64_t(fl) << (j & 31);
                ++length[j >> 5];
            }
            for(int j = 0; j < v.size(); ++j)
                v[j] = (j < v1.size() ? v1[j] : v2[j - v1.size()]);

            if(b.size() >= 2 && !(n & 31)){
                b[b.size() - 2] |= b[b.size() - 1] << 32;
                b.pop_back();
            }
            bv[i].build(n, b);
        }
    }

    void insert(int k, T x){
        for(int i = W - 1; i >= 0; --i){
            bool fl = (x >> i) & 1;
            bv[i].insert(k, fl);
            k = (fl ? bv[i].rank_val : k - bv[i].rank_val) + (fl ? bv[i].zero_cnt() : 0);
        }
    }

    void erase(int k){
        for(int i = W - 1; i >= 0; --i){
            int zero_cnt = bv[i].zero_cnt();
            bv[i].erase(k);
            bool fl = bv[i].erase_fl;
            int rank = (fl ? bv[i].rank_val : k - bv[i].rank_val);
            k = rank + (fl ? zero_cnt : 0);
        }
    }

    // [l, r)内のxの数
    int count(int l, int r, T x){
        for(int i = W - 1; i >= 0; --i){
            bool fl = (x >> i) & 1;
            int st = bv[i].rank(l, fl);
            int en = bv[i].rank(r, fl);
            l = (fl ? bv[i].zero_cnt() : 0) + st;
            r = (fl ? bv[i].zero_cnt() : 0) + en;
        }
        return r - l;
    }

    // [l, r)内で[0, x)を満たす値の数
    int count_lower(int l, int r, T x){
        int cnt = 0;
        for(int i = W - 1; i >= 0; --i){
            bool fl = (x >> i) & 1;
            int st = bv[i].rank(l, fl);
            int en = bv[i].rank(r, fl);
            if(fl){
                st += bv[i].zero_cnt();
                en += bv[i].zero_cnt();
                cnt += (bv[i].rank(r, 0) - bv[i].rank(l, 0));
            }
            l = st, r = en;
        }
        return cnt;
    }

    // [l, r)内で[x, y)を満たす値の数
    int count_range(int l, int r, T x, T y){
        return count_lower(l, r, y) - count_lower(l, r, x);
    }

    // 0-indexedでk番目に小さいものを返す
    T kth_min(int l, int r, int k){
        T ans = 0;
        for(int i = W - 1; i >= 0; --i){
            int st = bv[i].rank(l, 0);
            int en = bv[i].rank(r, 0);
            if(en - st <= k){
                k -= en - st;
                l = bv[i].zero_cnt() + (l - st);
                r = bv[i].zero_cnt() + (r - en);
                ans |= (1uLL << i);
            }
            else{
                l = st, r = en;
            }
        }
        return ans;
    }

    // [l, r)でのx以上最小値
    pair<T, bool> predecessor(int l, int r, T x){
        int idx = count_lower(l, r, x);
        if(idx == r - l){
            return make_pair((1uLL << W) - 1, false);
        }
        return make_pair(kth_min(l, r, idx), true);
    }

    // [l, r)でのx以下最大値
    pair<T, bool> successor(int l, int r, T x){
        int idx = count_lower(l, r, x + 1);
        if(idx == 0)
            return make_pair(0, false);
        return make_pair(kth_min(l, r, idx - 1), true);
    }
};

