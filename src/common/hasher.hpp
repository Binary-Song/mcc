#pragma once 
#include "common/precompiled.hpp"
#include "common/utility/functions/io.hpp"
#include <boost/compute/detail/sha1.hpp>
namespace mcc {

    class Hasher
    {
    public:
        enum class Algorithm
        {
            SHA1,

        } algorithm;

        Hasher(Algorithm algorithm = Algorithm::SHA1)
        {
            this->algorithm = algorithm;
        }

        String compute_hash(String const& string_to_hash)
        {
            if (algorithm == Algorithm::SHA1) {
                using namespace boost::compute::detail;
                sha1 hasher(string_to_hash);
                return hasher;
            }
            return "";
        }
    };
} // namespace mcc
