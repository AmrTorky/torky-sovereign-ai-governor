#ifndef TORKY_SOVEREIGN_AI_GOVERNOR_HPP
#define TORKY_SOVEREIGN_AI_GOVERNOR_HPP

#include <cmath>
#include <cstdint>
#include <string>
#include <algorithm>

namespace torky::governance::ai {

    inline constexpr double EPSILON_NM2       = 0.016;
    inline constexpr double V_FINITE_ZERO_EV  = 11.384;
    inline constexpr double COMPLIANCE_THRESHOLD = 0.85;

    #pragma pack(push, 1)
    struct TorkyCellDtype {
        uint8_t  gov_gate;     // TSG Gate Register (0x01 - 0x04)
        int16_t  phase_q16;    // Phase Q16 Fixed-Point
        int16_t  field_q16;    // Field Q16 Fixed-Point
        uint32_t syndrome_u32; // SHA-256 Merkle Syndrome
    };
    #pragma pack(pop)

    static_assert(sizeof(TorkyCellDtype) == 9, "CRITICAL: TorkyCellDtype must be precisely 9 bytes!");

    class SovereignAiGovernorEngine {
    public:
        // Evaluates AI inference requests against dynamic legal/ethical compliance matrices
        static inline uint8_t evaluate_inference_request(double compliance_score, bool security_flag, bool bit_tampered) {
            if (bit_tampered) return 0x03; // DENY
            if (!security_flag) return 0x02; // HOLD
            if (compliance_score >= COMPLIANCE_THRESHOLD) {
                return 0x01; // PROCEED
            } else if (compliance_score >= 0.50) {
                return 0x02; // HOLD
            } else {
                return 0x03; // DENY
            }
        }

        static inline double evaluate_v_finite(double r_nm) {
            return 1.0 / std::sqrt((r_nm * r_nm) + EPSILON_NM2);
        }
    };

    class TsgGateController {
    public:
        static inline bool validate_authorization(uint8_t gate_request, bool bit_tampered) {
            if (bit_tampered) return false;
            return (gate_request >= 0x01 && gate_request <= 0x04);
        }
    };

} // namespace torky::governance::ai

#endif // TORKY_SOVEREIGN_AI_GOVERNOR_HPP
