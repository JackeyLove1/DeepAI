class Solution {
public:
    vector<double> sampleStats(vector<int> &count) {
        auto sz = 0;
        std::map<int, int> freqs{};
        double sum = 0.0;
        int mode = -1, nums = 0;
        for(int i = 0; i <= 255; ++i){
            sum += static_cast<double>(i) * count[i];
            if (count[i]){
                freqs[i] = count[i];
                sz += count[i];
            }
            if (count[i] > nums){
                nums = count[i];
                mode = i;
            }
        }
        double minv = static_cast<double>(freqs.begin()->first);
        double maxv = static_cast<double>((--freqs.end())->first);
        double avg = sum / static_cast<double>(sz);
        double median = 0.0;
        if (sz % 2 == 0){
            uint32_t left = sz / 2 - 1, right = sz / 2 + 1, cur = 0;
            double leftv = 0.0, rightv = 0.0;
            bool find_left = false, find_right = false;
            for (const auto &[k, v]: freqs) {
                cur += v;
                if (!find_left && cur >= left) {
                    leftv = static_cast<double>(k);
                    find_left = true;
                }
                if (!find_right && cur >= right) {
                    rightv = static_cast<double>(k);
                    find_right = true;
                }
                if (find_left && find_right) break;
            }
            median = (leftv + rightv) / 2;
        } else {
            uint32_t cur =0;
            for (const auto &[k, v]: freqs){
                cur += v;
                if (cur > (sz / 2)){
                    median = k;
                    break;
                }
            }
        }
        return {minv, maxv, avg, median, static_cast<double>(mode)};
    }
};