struct SubstrMatching{
    int n;
    string s;
    vector<int> sa, lcp, lcp_table;
    SubstrMatching(string s) : s(s), n(s.size()){
        sa_is();
        lcp_build();
    }
    vector<int> induced_sort(vector<int> v, int k){
        if(v.size() == k){
            vector<int> ret(v.size());
            for(int i = 0; i < v.size(); ++i)
                ret[v[i]] = i;
            return ret;
        }
        vector<int> type(v.size(), 1);
        for(int i = v.size() - 2; i >= 0; --i)
            type[i] = (v[i] == v[i + 1] ? type[i + 1] : v[i] < v[i + 1]);
        vector<int> lms;
        vector<vector<int>> lms_str;
        for(int i = 0; i < v.size() - 1; ++i){
            if(!lms_str.empty())
                lms_str.back().emplace_back(v[i + 1]);
            if(!type[i] && type[i + 1]){
                lms_str.emplace_back(1, v[i + 1]);
                type[i + 1] = 2 + lms.size();
                lms.emplace_back(i + 1);
            }
        }
        vector<int> v_cnt(k, 0);
        for(auto x : v)
            ++v_cnt[x];
        vector<int> bin(k + 1, v.size());
        for(int i = 0, idx = 0; i < k; ++i){
            bin[i] = idx;
            idx += v_cnt[i];
        }
        auto calc = [&](auto& seed){
            vector<int> cnt(k, 0);
            vector<int> sa_v(v.size(), -1);
            for(auto i : seed){
                int ch = v[i];
                sa_v[bin[ch + 1] - cnt[ch] - 1] = i;
                ++cnt[ch];
            }
            cnt.assign(k, 0);
            for(int i = 0; i < v.size(); ++i){
                int nex = sa_v[i] - 1;
                if(nex >= 0 && type[nex] == 0){
                    int ch = v[nex];
                    sa_v[bin[ch] + cnt[ch]] = nex;
                    ++cnt[ch];
                }
            }
            cnt.assign(k, 0);

            for(int i = v.size() - 1; i >= 0; --i){
                int nex = sa_v[i] - 1;
                if(nex < 0 || !type[nex])
                    continue;
                if(nex >= 0 && type[nex]){
                    int ch = v[nex];
                    sa_v[bin[ch + 1] - cnt[ch] - 1] = nex;
                    ++cnt[ch];
                }
            }
            return sa_v;
        };
        auto ret_sa = calc(lms);
        int m = lms.size();
        vector<int> lms_idx;
        for(int i = 0; i < v.size(); ++i){
            if(type[ret_sa[i]] >= 2)
                lms_idx.emplace_back(type[ret_sa[i]] - 2);
        }
        int lms_cnt = 0;
        vector<int> lms_id(m, -100);
        for(int i = 0; i < m; ++i){
            if(i && lms_str[lms_idx[i - 1]] != lms_str[lms_idx[i]])
                ++lms_cnt;
            lms_id[lms_idx[i]] = lms_cnt + 0;
        }
        vector<int> ret_seed = induced_sort(lms_id, lms_cnt + 1);
        vector<int> seed(m, 0);
        for(int i = 0; i < m; ++i)
            seed[i] = lms[ret_seed[m - i - 1]];
        ret_sa = calc(seed);
        return ret_sa;
    }
    void sa_is(){
        unordered_set<char> c_uset;
        for(int i = 0; i < n; ++i)
            c_uset.insert(s[i]);
        set<char> c_set;
        for(auto c : c_uset)
            c_set.insert(c);
        unordered_map<int, int> c_idx;
        int k = 1;
        for(auto c : c_set)
            c_idx[c] = k++;
        vector<int> v(n + 1, 0);
        for(int i = 0; i < n; ++i)
            v[i] = c_idx[s[i]];
        sa = induced_sort(v, k);
    }
    void lcp_build(){
        vector<int> sa_inv(n + 1);
        for(int i = 0; i <= n; ++i)
            sa_inv[sa[i]] = i;
        lcp.assign(n + 1, 0);
        for(int i = 0, p = 0; i <= n; ++i){
            if(sa_inv[i] == n)
                continue;
            for(; i + p < n && sa[sa_inv[i] + 1] + p < n && s[i + p] == s[sa[sa_inv[i] + 1] + p]; ++p);
            lcp[sa_inv[i]] = p;
            if(p > 0)
                --p;
        }
        int siz = 1;
        for(; siz <= n; siz *= 2);
        lcp_table.resize(2 * siz, -1);
        for(int i = 0; i < n; ++i)
            lcp_table[i + siz] = lcp[i];
        for(int i = siz - 1; i > 0; --i)
            lcp_table[i] = min(lcp_table[i << 1], lcp_table[(i << 1) | 1]);
    }
    int match_len(string& t){
        int l = 0, r = lcp_table.size() / 2;
        int l_lcp = 0;
        int idx = 1;
        while(r - l > 1){
            int mid = (l + r) >> 1;
            int m_lcp = lcp_table[idx <<= 1];
            if(m_lcp == lcp_table[0])
                r = mid;
            else if(l_lcp < m_lcp){
                l = mid;
                ++idx;
            }
            else if(l_lcp > m_lcp)
                r = mid;
            else{
                for(m_lcp = l_lcp; m_lcp < t.size() && sa[mid] + m_lcp < s.size() && t[m_lcp] == s[sa[mid] + m_lcp]; ++m_lcp);
                if(sa[mid] + m_lcp == s.size() || m_lcp == t.size() || s[sa[mid] + m_lcp] < t[m_lcp]){
                    l_lcp = m_lcp;
                    l = mid;
                    ++idx;
                }else{
                    r = mid;
                }
            }
        }
        return l_lcp;
    }
    bool contains(string& t){
        return match_len(t) == t.size();
    }
    int lower_bound(string& t){
        int l = 0, r = lcp_table.size() / 2;
        int l_lcp = 0;
        int idx = 1;
        while(r - l > 1){
            int mid = (l + r) >> 1;
            int m_lcp = lcp_table[idx <<= 1];

            if(m_lcp == lcp_table[0])
                r = mid;
            else if(l_lcp < m_lcp){
                l = mid;
                ++idx;
            }
            else if(l_lcp > m_lcp)
                r = mid;
            else{
                for(m_lcp = l_lcp; m_lcp < t.size() && sa[mid] + m_lcp < s.size() && t[m_lcp] == s[sa[mid] + m_lcp]; ++m_lcp);
                if(sa[mid] + m_lcp == s.size() || m_lcp == t.size() || s[sa[mid] + m_lcp] < t[m_lcp]){
                    l_lcp = m_lcp;
                    l = mid;
                    ++idx;
                }else{
                    r = mid;
                }
            }
        }
        return r;
    }
    pair<int,int> find(string t){
        --t.back();
        auto l = lower_bound(t);
        ++t.back();
        auto r = lower_bound(t);
        return make_pair(l, r);
    }
};
