/**
 * @file gzipp_config.hpp
 * @author Simon Cahill (s.cahill@procyon-systems.de)
 * @brief Contains basic library configurations.
 * @version 0.1
 * @date 2025-04-25
 * 
 * @copyright Copyright (c) 2025 Procyon Systems All Rights Reserved
 * @license BSD-2-Clause
 */

#ifndef GZIPP_INCLUDE_GZIPP_GZIPP_CONFIG_HPP
#define GZIPP_INCLUDE_GZIPP_GZIPP_CONFIG_HPP

/////////////////////////////////
//       SYSTEM INCLUDES       //
/////////////////////////////////
// stl
#include <stdint.h>
#include <cstddef>

namespace gzipp {

    constexpr size_t DEFAULT_MAX_COMPRESSABLE_SIZE = 1024u * 1024u * 1024u * 2u; // 2 GiB

}

#endif // GZIPP_INCLUDE_GZIPP_GZIPP_CONFIG_HPP