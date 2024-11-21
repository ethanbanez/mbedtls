/**
 * \file mbedtls/config_adjust_legacy_from_psa.h
 * \brief Adjust PSA configuration: activate legacy implementations
 *
 * This is an internal header. Do not include it directly.
 *
 * Activate legacy implementations of cryptographic mechanisms as needed to
 * fulfill the needs of the PSA configuration. Generally speaking, we activate
 * a legacy mechanism if it's needed for a requested PSA mechanism and there is
 * no PSA driver for it.
 */
/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */

#ifndef MBEDTLS_CONFIG_ADJUST_LEGACY_FROM_PSA_H
#define MBEDTLS_CONFIG_ADJUST_LEGACY_FROM_PSA_H

#if !defined(MBEDTLS_CONFIG_FILES_READ)
#error "Do not include mbedtls/config_adjust_*.h manually! This can lead to problems, " \
    "up to and including runtime errors such as buffer overflows. " \
    "If you're trying to fix a complaint from check_config.h, just remove " \
    "it from your configuration file: since Mbed TLS 3.0, it is included " \
    "automatically at the right point."
#endif /* */

/* Define appropriate ACCEL macros for the p256-m driver.
 * In the future, those should be generated from the drivers JSON description.
 */
#if defined(MBEDTLS_PSA_P256M_DRIVER_ENABLED)
#define MBEDTLS_PSA_ACCEL_ECC_SECP_R1_256
#define MBEDTLS_PSA_ACCEL_ALG_ECDSA
#define MBEDTLS_PSA_ACCEL_ALG_ECDH
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_PUBLIC_KEY
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_BASIC
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_IMPORT
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_EXPORT
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_GENERATE
#endif

/*
 * ECC: support for a feature is controlled by a triplet or a pair:
 * (curve, key_type public/basic, alg) or (curve, key_type_<action>).
 *
 * A triplet/pair is accelerated if all of is components are accelerated;
 * otherwise each component needs to be built in.
 *
 * We proceed in two passes:
 * 1. Check if acceleration is complete for curves, key types, algs.
 * 2. Then enable built-ins for each thing that's either not accelerated of
 * doesn't have complete acceleration of the other triplet/pair components.
 *
 * Note: this needs psa/crypto_adjust_keypair_types.h to have been included
 * already, so that we know the full set of key types that are requested.
 */

/* ECC: curves: is acceleration complete? */
#if (defined(PSA_WANT_ECC_BRAINPOOL_P_R1_256) && \
    !defined(MBEDTLS_PSA_ACCEL_ECC_BRAINPOOL_P_R1_256)) || \
    (defined(PSA_WANT_ECC_BRAINPOOL_P_R1_384) && \
    !defined(MBEDTLS_PSA_ACCEL_ECC_BRAINPOOL_P_R1_384)) || \
    (defined(PSA_WANT_ECC_BRAINPOOL_P_R1_512) && \
    !defined(MBEDTLS_PSA_ACCEL_ECC_BRAINPOOL_P_R1_512)) || \
    (defined(PSA_WANT_ECC_SECP_R1_192) && !defined(MBEDTLS_PSA_ACCEL_ECC_SECP_R1_192)) || \
    (defined(PSA_WANT_ECC_SECP_R1_224) && !defined(MBEDTLS_PSA_ACCEL_ECC_SECP_R1_224)) || \
    (defined(PSA_WANT_ECC_SECP_R1_256) && !defined(MBEDTLS_PSA_ACCEL_ECC_SECP_R1_256)) || \
    (defined(PSA_WANT_ECC_SECP_R1_384) && !defined(MBEDTLS_PSA_ACCEL_ECC_SECP_R1_384)) || \
    (defined(PSA_WANT_ECC_SECP_R1_521) && !defined(MBEDTLS_PSA_ACCEL_ECC_SECP_R1_521)) || \
    (defined(PSA_WANT_ECC_SECP_K1_192) && !defined(MBEDTLS_PSA_ACCEL_ECC_SECP_K1_192)) || \
    (defined(PSA_WANT_ECC_SECP_K1_224) && !defined(MBEDTLS_PSA_ACCEL_ECC_SECP_K1_224)) || \
    (defined(PSA_WANT_ECC_SECP_K1_256) && !defined(MBEDTLS_PSA_ACCEL_ECC_SECP_K1_256))
#define MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_CURVES
#define MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_WEIERSTRASS_CURVES
#endif

#if (defined(PSA_WANT_ECC_MONTGOMERY_255) && !defined(MBEDTLS_PSA_ACCEL_ECC_MONTGOMERY_255)) || \
    (defined(PSA_WANT_ECC_MONTGOMERY_448) && !defined(MBEDTLS_PSA_ACCEL_ECC_MONTGOMERY_448))
#define MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_CURVES
#endif

/* ECC: algs: is acceleration complete? */
#if (defined(PSA_WANT_ALG_ECDH) && !defined(MBEDTLS_PSA_ACCEL_ALG_ECDH)) || \
    (defined(PSA_WANT_ALG_ECDSA) && !defined(MBEDTLS_PSA_ACCEL_ALG_ECDSA)) || \
    (defined(PSA_WANT_ALG_DETERMINISTIC_ECDSA) && \
    !defined(MBEDTLS_PSA_ACCEL_ALG_DETERMINISTIC_ECDSA)) || \
    (defined(PSA_WANT_ALG_JPAKE) && !defined(MBEDTLS_PSA_ACCEL_ALG_JPAKE))
#define MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_ALGS
#endif

/* ECC: key types: is acceleration complete? */
#if (defined(PSA_WANT_KEY_TYPE_ECC_PUBLIC_KEY) && \
    !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_PUBLIC_KEY)) || \
    (defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_BASIC) && \
    !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_BASIC))
#define MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES_BASIC
#endif

/* Special case: we don't support cooked key derivation in drivers yet */
#if defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_DERIVE)
#undef MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_DERIVE
#endif

/* Note: the condition about key derivation is always true as DERIVE can't be
 * accelerated yet */
#if (defined(PSA_WANT_KEY_TYPE_ECC_PUBLIC_KEY) && \
    !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_PUBLIC_KEY)) || \
    (defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_BASIC) && \
    !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_BASIC)) || \
    (defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_IMPORT) && \
    !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_IMPORT)) || \
    (defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_EXPORT) && \
    !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_EXPORT)) || \
    (defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_GENERATE) && \
    !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_GENERATE)) || \
    (defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_DERIVE) && \
    !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_DERIVE))
#define MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES
#endif

/* ECC: curves: enable built-ins as needed.
 *
 * We need the curve built-in:
 * - if it's not accelerated, or
 * - if there's a key type with missing acceleration, or
 * - if there's a alg with missing acceleration.
 */
#if defined(PSA_WANT_ECC_BRAINPOOL_P_R1_256)
#if !defined(MBEDTLS_PSA_ACCEL_ECC_BRAINPOOL_P_R1_256) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_ECC_BRAINPOOL_P_R1_256 1
#define MBEDTLS_ECP_DP_BP256R1_ENABLED
#endif /* missing accel */
#endif /* PSA_WANT_ECC_BRAINPOOL_P_R1_256 */

#if defined(PSA_WANT_ECC_BRAINPOOL_P_R1_384)
#if !defined(MBEDTLS_PSA_ACCEL_ECC_BRAINPOOL_P_R1_384) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_ECC_BRAINPOOL_P_R1_384 1
#define MBEDTLS_ECP_DP_BP384R1_ENABLED
#endif /* missing accel */
#endif /* PSA_WANT_ECC_BRAINPOOL_P_R1_384 */

#if defined(PSA_WANT_ECC_BRAINPOOL_P_R1_512)
#if !defined(MBEDTLS_PSA_ACCEL_ECC_BRAINPOOL_P_R1_512) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_ECC_BRAINPOOL_P_R1_512 1
#define MBEDTLS_ECP_DP_BP512R1_ENABLED
#endif /* missing accel */
#endif /* PSA_WANT_ECC_BRAINPOOL_P_R1_512 */

#if defined(PSA_WANT_ECC_MONTGOMERY_255)
#if !defined(MBEDTLS_PSA_ACCEL_ECC_MONTGOMERY_255) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_ECC_MONTGOMERY_255 1
#define MBEDTLS_ECP_DP_CURVE25519_ENABLED
#endif /* missing accel */
#endif /* PSA_WANT_ECC_MONTGOMERY_255 */

#if defined(PSA_WANT_ECC_MONTGOMERY_448)
#if !defined(MBEDTLS_PSA_ACCEL_ECC_MONTGOMERY_448) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_ECC_MONTGOMERY_448 1
#define MBEDTLS_ECP_DP_CURVE448_ENABLED
#endif /* missing accel */
#endif /* PSA_WANT_ECC_MONTGOMERY_448 */

#if defined(PSA_WANT_ECC_SECP_R1_192)
#if !defined(MBEDTLS_PSA_ACCEL_ECC_SECP_R1_192) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_ECC_SECP_R1_192 1
#define MBEDTLS_ECP_DP_SECP192R1_ENABLED
#endif /* missing accel */
#endif /* PSA_WANT_ECC_SECP_R1_192 */

#if defined(PSA_WANT_ECC_SECP_R1_224)
#if !defined(MBEDTLS_PSA_ACCEL_ECC_SECP_R1_224) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_ECC_SECP_R1_224 1
#define MBEDTLS_ECP_DP_SECP224R1_ENABLED
#endif /* missing accel */
#endif /* PSA_WANT_ECC_SECP_R1_224 */

#if defined(PSA_WANT_ECC_SECP_R1_256)
#if !defined(MBEDTLS_PSA_ACCEL_ECC_SECP_R1_256) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_ECC_SECP_R1_256 1
#define MBEDTLS_ECP_DP_SECP256R1_ENABLED
#endif /* missing accel */
#endif /* PSA_WANT_ECC_SECP_R1_256 */

#if defined(PSA_WANT_ECC_SECP_R1_384)
#if !defined(MBEDTLS_PSA_ACCEL_ECC_SECP_R1_384) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_ECC_SECP_R1_384 1
#define MBEDTLS_ECP_DP_SECP384R1_ENABLED
#endif /* missing accel */
#endif /* PSA_WANT_ECC_SECP_R1_384 */

#if defined(PSA_WANT_ECC_SECP_R1_521)
#if !defined(MBEDTLS_PSA_ACCEL_ECC_SECP_R1_521) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_ECC_SECP_R1_521 1
#define MBEDTLS_ECP_DP_SECP521R1_ENABLED
#endif /* missing accel */
#endif /* PSA_WANT_ECC_SECP_R1_521 */

#if defined(PSA_WANT_ECC_SECP_K1_192)
#if !defined(MBEDTLS_PSA_ACCEL_ECC_SECP_K1_192) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_ECC_SECP_K1_192 1
#define MBEDTLS_ECP_DP_SECP192K1_ENABLED
#endif /* missing accel */
#endif /* PSA_WANT_ECC_SECP_K1_192 */

#if defined(PSA_WANT_ECC_SECP_K1_224)
#if !defined(MBEDTLS_PSA_ACCEL_ECC_SECP_K1_224) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_ECC_SECP_K1_224 1
#define MBEDTLS_ECP_DP_SECP224K1_ENABLED
/* https://github.com/Mbed-TLS/mbedtls/issues/3541 */
#error "SECP224K1 is buggy via the PSA API in Mbed TLS."
#endif /* missing accel */
#endif /* PSA_WANT_ECC_SECP_K1_224 */

#if defined(PSA_WANT_ECC_SECP_K1_256)
#if !defined(MBEDTLS_PSA_ACCEL_ECC_SECP_K1_256) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_ECC_SECP_K1_256 1
#define MBEDTLS_ECP_DP_SECP256K1_ENABLED
#endif /* missing accel */
#endif /* PSA_WANT_ECC_SECP_K1_256 */

/* ECC: algs: enable built-ins as needed.
 *
 * We need the alg built-in:
 * - if it's not accelerated, or
 * - if there's a relevant curve (see below) with missing acceleration, or
 * - if there's a key type among (public, basic) with missing acceleration.
 *
 * Relevant curves are:
 * - all curves for ECDH
 * - Weierstrass curves for (deterministic) ECDSA
 * - secp256r1 for EC J-PAKE
 */
#if defined(PSA_WANT_ALG_DETERMINISTIC_ECDSA)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_DETERMINISTIC_ECDSA) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_WEIERSTRASS_CURVES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES_BASIC)
#define MBEDTLS_PSA_BUILTIN_ALG_DETERMINISTIC_ECDSA 1
#define MBEDTLS_ECDSA_DETERMINISTIC
#define MBEDTLS_HMAC_DRBG_C
#define MBEDTLS_MD_C
#define MBEDTLS_ECDSA_C
#define MBEDTLS_ECP_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_ASN1_WRITE_C
#endif /* missing accel */
#endif /* PSA_WANT_ALG_DETERMINISTIC_ECDSA */

#if defined(PSA_WANT_ALG_ECDH)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_ECDH) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_CURVES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES_BASIC)
#define MBEDTLS_PSA_BUILTIN_ALG_ECDH 1
#define MBEDTLS_ECDH_C
#define MBEDTLS_ECP_C
#define MBEDTLS_BIGNUM_C
#endif /* missing accel */
#endif /* PSA_WANT_ALG_ECDH */

#if defined(PSA_WANT_ALG_ECDSA)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_ECDSA) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_WEIERSTRASS_CURVES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES_BASIC)
#define MBEDTLS_PSA_BUILTIN_ALG_ECDSA 1
#define MBEDTLS_ECDSA_C
#define MBEDTLS_ECP_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_ASN1_WRITE_C
#endif /* missing accel */
#endif /* PSA_WANT_ALG_ECDSA */

#if defined(PSA_WANT_ALG_JPAKE)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_JPAKE) || \
    !defined(MBEDTLS_PSA_ACCEL_ECC_SECP_R1_256) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_KEY_TYPES_BASIC)
#define MBEDTLS_PSA_BUILTIN_PAKE 1
#define MBEDTLS_PSA_BUILTIN_ALG_JPAKE 1
#define MBEDTLS_ECP_DP_SECP256R1_ENABLED
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_ECP_C
#define MBEDTLS_ECJPAKE_C
#endif /* missing accel */
#endif /* PSA_WANT_ALG_JPAKE */

/* ECC: key types: enable built-ins as needed.
 *
 * We need the key type built-in:
 * - if it's not accelerated, or
 * - if there's a curve with missing acceleration, or
 * - only for public/basic: if there's an alg with missing acceleration.
 */
#if defined(PSA_WANT_KEY_TYPE_ECC_PUBLIC_KEY)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_PUBLIC_KEY) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_CURVES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_ECC_PUBLIC_KEY 1
#endif /* missing accel */
#endif /* PSA_WANT_KEY_TYPE_ECC_PUBLIC_KEY */

#if defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_BASIC)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_BASIC) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_CURVES) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_ECC_KEY_PAIR_BASIC 1
#endif /* missing accel */
#endif /* PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_BASIC */

#if defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_IMPORT)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_IMPORT) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_CURVES)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_ECC_KEY_PAIR_IMPORT 1
#endif /* missing accel */
#endif /* PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_IMPORT */

#if defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_EXPORT)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_EXPORT) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_CURVES)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_ECC_KEY_PAIR_EXPORT 1
#endif /* missing accel */
#endif /* PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_EXPORT */

#if defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_GENERATE)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_GENERATE) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_CURVES)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_ECC_KEY_PAIR_GENERATE 1
#endif /* missing accel */
#endif /* PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_GENERATE */

/* Note: the condition is always true as DERIVE can't be accelerated yet */
#if defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_DERIVE)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_DERIVE) || \
    defined(MBEDTLS_PSA_ECC_ACCEL_INCOMPLETE_CURVES)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_ECC_KEY_PAIR_DERIVE 1
#endif /* missing accel */
#endif /* !MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_DERIVE */

#if defined(MBEDTLS_PSA_BUILTIN_KEY_TYPE_ECC_PUBLIC_KEY) || \
    defined(MBEDTLS_PSA_BUILTIN_KEY_TYPE_ECC_KEY_PAIR_BASIC) || \
    defined(MBEDTLS_PSA_BUILTIN_KEY_TYPE_ECC_KEY_PAIR_IMPORT) || \
    defined(MBEDTLS_PSA_BUILTIN_KEY_TYPE_ECC_KEY_PAIR_DERIVE)
#define MBEDTLS_ECP_LIGHT
#define MBEDTLS_BIGNUM_C
#endif

#if defined(MBEDTLS_PSA_BUILTIN_KEY_TYPE_ECC_KEY_PAIR_EXPORT) || \
    defined(MBEDTLS_PSA_BUILTIN_KEY_TYPE_ECC_KEY_PAIR_GENERATE)
#define MBEDTLS_ECP_C
#define MBEDTLS_BIGNUM_C
#endif

/* End of ECC section */

/*
 * DH key types follow the same pattern used above for EC keys. They are defined
 * by a triplet (group, key_type, alg). A triplet is accelerated if all its
 * component are accelerated, otherwise each component needs to be builtin.
 */

/* DH: groups: is acceleration complete? */
#if (defined(PSA_WANT_DH_RFC7919_2048) && !defined(MBEDTLS_PSA_ACCEL_DH_RFC7919_2048)) || \
    (defined(PSA_WANT_DH_RFC7919_3072) && !defined(MBEDTLS_PSA_ACCEL_DH_RFC7919_3072)) || \
    (defined(PSA_WANT_DH_RFC7919_4096) && !defined(MBEDTLS_PSA_ACCEL_DH_RFC7919_4096)) || \
    (defined(PSA_WANT_DH_RFC7919_6144) && !defined(MBEDTLS_PSA_ACCEL_DH_RFC7919_6144)) || \
    (defined(PSA_WANT_DH_RFC7919_8192) && !defined(MBEDTLS_PSA_ACCEL_DH_RFC7919_8192))
#define MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_GROUPS
#endif

/* DH: algs: is acceleration complete? */
#if defined(PSA_WANT_ALG_FFDH) && !defined(MBEDTLS_PSA_ACCEL_ALG_FFDH)
#define MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_ALGS
#endif

/* DH: key types: is acceleration complete? */
#if (defined(PSA_WANT_KEY_TYPE_DH_PUBLIC_KEY) && \
    !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_PUBLIC_KEY)) || \
    (defined(PSA_WANT_KEY_TYPE_DH_KEY_PAIR_BASIC) && \
    !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_KEY_PAIR_BASIC)) || \
    (defined(PSA_WANT_KEY_TYPE_DH_KEY_PAIR_IMPORT) && \
    !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_KEY_PAIR_IMPORT)) || \
    (defined(PSA_WANT_KEY_TYPE_DH_KEY_PAIR_EXPORT) && \
    !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_KEY_PAIR_EXPORT)) || \
    (defined(PSA_WANT_KEY_TYPE_DH_KEY_PAIR_GENERATE) && \
    !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_KEY_PAIR_GENERATE))
#define MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_KEY_TYPES
#endif

#if defined(PSA_WANT_DH_RFC7919_2048)
#if !defined(MBEDTLS_PSA_ACCEL_DH_RFC7919_2048) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_ALGS) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_KEY_TYPES)
#define MBEDTLS_PSA_BUILTIN_DH_RFC7919_2048 1
#endif /* !MBEDTLS_PSA_BUILTIN_DH_RFC7919_2048 */
#endif /* PSA_WANT_DH_RFC7919_2048 */

#if defined(PSA_WANT_DH_RFC7919_3072)
#if !defined(MBEDTLS_PSA_ACCEL_DH_RFC7919_3072) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_ALGS) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_KEY_TYPES)
#define MBEDTLS_PSA_BUILTIN_DH_RFC7919_3072 1
#endif /* !MBEDTLS_PSA_BUILTIN_DH_RFC7919_3072 */
#endif /* PSA_WANT_DH_RFC7919_3072 */

#if defined(PSA_WANT_DH_RFC7919_4096)
#if !defined(MBEDTLS_PSA_ACCEL_DH_RFC7919_4096) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_ALGS) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_KEY_TYPES)
#define MBEDTLS_PSA_BUILTIN_DH_RFC7919_4096 1
#endif /* !MBEDTLS_PSA_BUILTIN_DH_RFC7919_4096 */
#endif /* PSA_WANT_DH_RFC7919_4096 */

#if defined(PSA_WANT_DH_RFC7919_6144)
#if !defined(MBEDTLS_PSA_ACCEL_DH_RFC7919_6144) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_ALGS) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_KEY_TYPES)
#define MBEDTLS_PSA_BUILTIN_DH_RFC7919_6144 1
#endif /* !MBEDTLS_PSA_BUILTIN_DH_RFC7919_6144 */
#endif /* PSA_WANT_DH_RFC7919_6144 */

#if defined(PSA_WANT_DH_RFC7919_8192)
#if !defined(MBEDTLS_PSA_ACCEL_DH_RFC7919_8192) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_ALGS) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_KEY_TYPES)
#define MBEDTLS_PSA_BUILTIN_DH_RFC7919_8192 1
#endif /* !MBEDTLS_PSA_BUILTIN_DH_RFC7919_8192 */
#endif /* PSA_WANT_DH_RFC7919_8192 */

#if defined(PSA_WANT_ALG_FFDH)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_FFDH) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_GROUPS) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_KEY_TYPES)
#define MBEDTLS_PSA_BUILTIN_ALG_FFDH 1
#define MBEDTLS_BIGNUM_C
#endif /* !MBEDTLS_PSA_ACCEL_ALG_FFDH */
#endif /* PSA_WANT_ALG_FFDH */

#if defined(PSA_WANT_KEY_TYPE_DH_KEY_PAIR_IMPORT)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_KEY_PAIR_IMPORT) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_GROUPS) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_KEY_PAIR_IMPORT 1
#endif /* !MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_KEY_PAIR_IMPORT */
#endif /* PSA_WANT_KEY_TYPE_DH_KEY_PAIR_IMPORT */

#if defined(PSA_WANT_KEY_TYPE_DH_KEY_PAIR_EXPORT)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_KEY_PAIR_EXPORT) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_GROUPS) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_KEY_PAIR_EXPORT 1
#endif /* !MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_KEY_PAIR_EXPORT */
#endif /* PSA_WANT_KEY_TYPE_DH_KEY_PAIR_EXPORT */

#if defined(PSA_WANT_KEY_TYPE_DH_KEY_PAIR_GENERATE)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_KEY_PAIR_GENERATE)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_KEY_PAIR_GENERATE 1
#endif /* !MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_KEY_PAIR_GENERATE */
#endif /* PSA_WANT_KEY_TYPE_DH_KEY_PAIR_GENERATE */

#if defined(PSA_WANT_KEY_TYPE_DH_KEY_PAIR_BASIC)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_KEY_PAIR_BASIC) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_GROUPS) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_KEY_PAIR_BASIC 1
#endif /* !MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_KEY_PAIR_BASIC */
#endif /* PSA_WANT_KEY_TYPE_DH_KEY_PAIR_BASIC */

#if defined(PSA_WANT_KEY_TYPE_DH_PUBLIC_KEY)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_PUBLIC_KEY) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_GROUPS) || \
    defined(MBEDTLS_PSA_DH_ACCEL_INCOMPLETE_ALGS)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_DH_PUBLIC_KEY 1
#define MBEDTLS_BIGNUM_C
#endif /* !MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_PUBLIC_KEY */
#endif /* PSA_WANT_KEY_TYPE_DH_PUBLIC_KEY */

/* End of DH section */

#if defined(PSA_WANT_ALG_HKDF)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_HKDF)
/*
 * The PSA implementation has its own implementation of HKDF, separate from
 * hkdf.c. No need to enable MBEDTLS_HKDF_C here.
 */
#define MBEDTLS_PSA_BUILTIN_ALG_HKDF 1
#endif /* !MBEDTLS_PSA_ACCEL_ALG_HKDF */
#endif /* PSA_WANT_ALG_HKDF */

#if defined(PSA_WANT_ALG_HKDF_EXTRACT)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_HKDF_EXTRACT)
/*
 * The PSA implementation has its own implementation of HKDF, separate from
 * hkdf.c. No need to enable MBEDTLS_HKDF_C here.
 */
#define MBEDTLS_PSA_BUILTIN_ALG_HKDF_EXTRACT 1
#endif /* !MBEDTLS_PSA_ACCEL_ALG_HKDF_EXTRACT */
#endif /* PSA_WANT_ALG_HKDF_EXTRACT */

#if defined(PSA_WANT_ALG_HKDF_EXPAND)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_HKDF_EXPAND)
/*
 * The PSA implementation has its own implementation of HKDF, separate from
 * hkdf.c. No need to enable MBEDTLS_HKDF_C here.
 */
#define MBEDTLS_PSA_BUILTIN_ALG_HKDF_EXPAND 1
#endif /* !MBEDTLS_PSA_ACCEL_ALG_HKDF_EXPAND */
#endif /* PSA_WANT_ALG_HKDF_EXPAND */

#if defined(PSA_WANT_ALG_HMAC)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_HMAC)
#define MBEDTLS_PSA_BUILTIN_ALG_HMAC 1
#endif /* !MBEDTLS_PSA_ACCEL_ALG_HMAC */
#endif /* PSA_WANT_ALG_HMAC */

#if defined(PSA_WANT_ALG_MD5) && !defined(MBEDTLS_PSA_ACCEL_ALG_MD5)
#define MBEDTLS_PSA_BUILTIN_ALG_MD5 1
#define MBEDTLS_MD5_C
#endif

#if defined(PSA_WANT_ALG_RIPEMD160) && !defined(MBEDTLS_PSA_ACCEL_ALG_RIPEMD160)
#define MBEDTLS_PSA_BUILTIN_ALG_RIPEMD160 1
#define MBEDTLS_RIPEMD160_C
#endif

#if defined(PSA_WANT_ALG_RSA_OAEP)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_RSA_OAEP)
#define MBEDTLS_PSA_BUILTIN_ALG_RSA_OAEP 1
#define MBEDTLS_RSA_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_OID_C
#define MBEDTLS_PKCS1_V21
#endif /* !MBEDTLS_PSA_ACCEL_ALG_RSA_OAEP */
#endif /* PSA_WANT_ALG_RSA_OAEP */

#if defined(PSA_WANT_ALG_RSA_PKCS1V15_CRYPT)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_RSA_PKCS1V15_CRYPT)
#define MBEDTLS_PSA_BUILTIN_ALG_RSA_PKCS1V15_CRYPT 1
#define MBEDTLS_RSA_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_OID_C
#define MBEDTLS_PKCS1_V15
#endif /* !MBEDTLS_PSA_ACCEL_ALG_RSA_PKCS1V15_CRYPT */
#endif /* PSA_WANT_ALG_RSA_PKCS1V15_CRYPT */

#if defined(PSA_WANT_ALG_RSA_PKCS1V15_SIGN)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_RSA_PKCS1V15_SIGN)
#define MBEDTLS_PSA_BUILTIN_ALG_RSA_PKCS1V15_SIGN 1
#define MBEDTLS_RSA_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_OID_C
#define MBEDTLS_PKCS1_V15
#endif /* !MBEDTLS_PSA_ACCEL_ALG_RSA_PKCS1V15_SIGN */
#endif /* PSA_WANT_ALG_RSA_PKCS1V15_SIGN */

#if defined(PSA_WANT_ALG_RSA_PSS)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_RSA_PSS)
#define MBEDTLS_PSA_BUILTIN_ALG_RSA_PSS 1
#define MBEDTLS_RSA_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_OID_C
#define MBEDTLS_PKCS1_V21
#endif /* !MBEDTLS_PSA_ACCEL_ALG_RSA_PSS */
#endif /* PSA_WANT_ALG_RSA_PSS */

#if defined(PSA_WANT_ALG_SHA_1) && !defined(MBEDTLS_PSA_ACCEL_ALG_SHA_1)
#define MBEDTLS_PSA_BUILTIN_ALG_SHA_1 1
#define MBEDTLS_SHA1_C
#endif

#if defined(PSA_WANT_ALG_SHA_224) && !defined(MBEDTLS_PSA_ACCEL_ALG_SHA_224)
#define MBEDTLS_PSA_BUILTIN_ALG_SHA_224 1
#define MBEDTLS_SHA224_C
#endif

#if defined(PSA_WANT_ALG_SHA_256) && !defined(MBEDTLS_PSA_ACCEL_ALG_SHA_256)
#define MBEDTLS_PSA_BUILTIN_ALG_SHA_256 1
#define MBEDTLS_SHA256_C
#endif

#if defined(PSA_WANT_ALG_SHA_384) && !defined(MBEDTLS_PSA_ACCEL_ALG_SHA_384)
#define MBEDTLS_PSA_BUILTIN_ALG_SHA_384 1
#define MBEDTLS_SHA384_C
#endif

#if defined(PSA_WANT_ALG_SHA_512) && !defined(MBEDTLS_PSA_ACCEL_ALG_SHA_512)
#define MBEDTLS_PSA_BUILTIN_ALG_SHA_512 1
#define MBEDTLS_SHA512_C
#endif

#if defined(PSA_WANT_ALG_SHA3_224) && !defined(MBEDTLS_PSA_ACCEL_ALG_SHA3_224)
#define MBEDTLS_PSA_BUILTIN_ALG_SHA3_224 1
#define MBEDTLS_SHA3_C
#endif

#if defined(PSA_WANT_ALG_SHA3_256) && !defined(MBEDTLS_PSA_ACCEL_ALG_SHA3_256)
#define MBEDTLS_PSA_BUILTIN_ALG_SHA3_256 1
#define MBEDTLS_SHA3_C
#endif

#if defined(PSA_WANT_ALG_SHA3_384) && !defined(MBEDTLS_PSA_ACCEL_ALG_SHA3_384)
#define MBEDTLS_PSA_BUILTIN_ALG_SHA3_384 1
#define MBEDTLS_SHA3_C
#endif

#if defined(PSA_WANT_ALG_SHA3_512) && !defined(MBEDTLS_PSA_ACCEL_ALG_SHA3_512)
#define MBEDTLS_PSA_BUILTIN_ALG_SHA3_512 1
#define MBEDTLS_SHA3_C
#endif

#if defined(PSA_WANT_ALG_PBKDF2_HMAC)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_PBKDF2_HMAC)
#define MBEDTLS_PSA_BUILTIN_ALG_PBKDF2_HMAC 1
#define PSA_HAVE_SOFT_PBKDF2_HMAC 1
#endif /* !MBEDTLS_PSA_BUILTIN_ALG_PBKDF2_HMAC */
#endif /* PSA_WANT_ALG_PBKDF2_HMAC */

#if defined(PSA_WANT_ALG_TLS12_PRF)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_TLS12_PRF)
#define MBEDTLS_PSA_BUILTIN_ALG_TLS12_PRF 1
#endif /* !MBEDTLS_PSA_ACCEL_ALG_TLS12_PRF */
#endif /* PSA_WANT_ALG_TLS12_PRF */

#if defined(PSA_WANT_ALG_TLS12_PSK_TO_MS)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_TLS12_PSK_TO_MS)
#define MBEDTLS_PSA_BUILTIN_ALG_TLS12_PSK_TO_MS 1
#endif /* !MBEDTLS_PSA_ACCEL_ALG_TLS12_PSK_TO_MS */
#endif /* PSA_WANT_ALG_TLS12_PSK_TO_MS */

#if defined(PSA_WANT_ALG_TLS12_ECJPAKE_TO_PMS)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_TLS12_ECJPAKE_TO_PMS)
#define MBEDTLS_PSA_BUILTIN_ALG_TLS12_ECJPAKE_TO_PMS 1
#endif /* !MBEDTLS_PSA_ACCEL_ALG_TLS12_ECJPAKE_TO_PMS */
#endif /* PSA_WANT_ALG_TLS12_ECJPAKE_TO_PMS */

#if defined(PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_IMPORT)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_KEY_PAIR_IMPORT)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_RSA_KEY_PAIR_IMPORT 1
#endif /* !MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_KEY_PAIR_IMPORT */
#endif /* PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_IMPORT */

#if defined(PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_EXPORT)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_KEY_PAIR_EXPORT)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_RSA_KEY_PAIR_EXPORT 1
#endif /* !MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_KEY_PAIR_EXPORT */
#endif /* PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_EXPORT */

#if defined(PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_GENERATE)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_KEY_PAIR_GENERATE)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_RSA_KEY_PAIR_GENERATE 1
#define MBEDTLS_GENPRIME
#endif /* !MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_KEY_PAIR_GENERATE */
#endif /* PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_GENERATE */

#if defined(PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_BASIC)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_KEY_PAIR_BASIC)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_RSA_KEY_PAIR_BASIC 1
#endif /* !MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_KEY_PAIR_BASIC */
#endif /* PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_BASIC */

#if defined(PSA_WANT_KEY_TYPE_RSA_PUBLIC_KEY)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_PUBLIC_KEY)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_RSA_PUBLIC_KEY 1
#define MBEDTLS_RSA_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_OID_C
#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_ASN1_WRITE_C
#endif /* !MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_PUBLIC_KEY */
#endif /* PSA_WANT_KEY_TYPE_RSA_PUBLIC_KEY */

/* If any of the block modes are requested that don't have an
 * associated HW assist, define PSA_HAVE_SOFT_BLOCK_MODE for checking
 * in the block cipher key types. */
#if (defined(PSA_WANT_ALG_CTR) && !defined(MBEDTLS_PSA_ACCEL_ALG_CTR)) || \
    (defined(PSA_WANT_ALG_CFB) && !defined(MBEDTLS_PSA_ACCEL_ALG_CFB)) || \
    (defined(PSA_WANT_ALG_OFB) && !defined(MBEDTLS_PSA_ACCEL_ALG_OFB)) || \
    (defined(PSA_WANT_ALG_ECB_NO_PADDING) && !defined(MBEDTLS_PSA_ACCEL_ALG_ECB_NO_PADDING)) || \
    (defined(PSA_WANT_ALG_CBC_NO_PADDING) && !defined(MBEDTLS_PSA_ACCEL_ALG_CBC_NO_PADDING)) || \
    (defined(PSA_WANT_ALG_CBC_PKCS7) && !defined(MBEDTLS_PSA_ACCEL_ALG_CBC_PKCS7)) || \
    (defined(PSA_WANT_ALG_CMAC) && !defined(MBEDTLS_PSA_ACCEL_ALG_CMAC))
#define PSA_HAVE_SOFT_BLOCK_MODE 1
#endif

#if defined(PSA_WANT_ALG_PBKDF2_AES_CMAC_PRF_128)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_PBKDF2_AES_CMAC_PRF_128)
#define MBEDTLS_PSA_BUILTIN_ALG_PBKDF2_AES_CMAC_PRF_128 1
#define PSA_HAVE_SOFT_PBKDF2_CMAC 1
#endif /* !MBEDTLS_PSA_ACCEL_ALG_PBKDF2_AES_CMAC_PRF_128 */
#endif /* PSA_WANT_ALG_PBKDF2_AES_CMAC_PRF_128 */

#if defined(PSA_WANT_KEY_TYPE_AES)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_AES)
#define PSA_HAVE_SOFT_KEY_TYPE_AES 1
#endif /* !MBEDTLS_PSA_ACCEL_KEY_TYPE_AES */
#if defined(PSA_HAVE_SOFT_KEY_TYPE_AES) || \
    defined(PSA_HAVE_SOFT_BLOCK_MODE)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_AES 1
#define MBEDTLS_AES_C
#endif /* PSA_HAVE_SOFT_KEY_TYPE_AES || PSA_HAVE_SOFT_BLOCK_MODE */
#endif /* PSA_WANT_KEY_TYPE_AES */

#if defined(PSA_WANT_KEY_TYPE_ARIA)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_ARIA)
#define PSA_HAVE_SOFT_KEY_TYPE_ARIA 1
#endif /* !MBEDTLS_PSA_ACCEL_KEY_TYPE_ARIA */
#if defined(PSA_HAVE_SOFT_KEY_TYPE_ARIA) || \
    defined(PSA_HAVE_SOFT_BLOCK_MODE)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_ARIA 1
#define MBEDTLS_ARIA_C
#endif /* PSA_HAVE_SOFT_KEY_TYPE_ARIA || PSA_HAVE_SOFT_BLOCK_MODE */
#endif /* PSA_WANT_KEY_TYPE_ARIA */

#if defined(PSA_WANT_KEY_TYPE_CAMELLIA)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_CAMELLIA)
#define PSA_HAVE_SOFT_KEY_TYPE_CAMELLIA 1
#endif /* !MBEDTLS_PSA_ACCEL_KEY_TYPE_CAMELLIA */
#if defined(PSA_HAVE_SOFT_KEY_TYPE_CAMELLIA) || \
    defined(PSA_HAVE_SOFT_BLOCK_MODE)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_CAMELLIA 1
#define MBEDTLS_CAMELLIA_C
#endif /* PSA_HAVE_SOFT_KEY_TYPE_CAMELLIA || PSA_HAVE_SOFT_BLOCK_MODE */
#endif /* PSA_WANT_KEY_TYPE_CAMELLIA */

#if defined(PSA_WANT_KEY_TYPE_DES)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_DES)
#define PSA_HAVE_SOFT_KEY_TYPE_DES 1
#endif /* !MBEDTLS_PSA_ACCEL_KEY_TYPE_DES */
#if defined(PSA_HAVE_SOFT_KEY_TYPE_DES) || \
    defined(PSA_HAVE_SOFT_BLOCK_MODE)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_DES 1
#define MBEDTLS_DES_C
#endif /*PSA_HAVE_SOFT_KEY_TYPE_DES || PSA_HAVE_SOFT_BLOCK_MODE */
#endif /* PSA_WANT_KEY_TYPE_DES */

#if defined(PSA_WANT_ALG_STREAM_CIPHER)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_STREAM_CIPHER)
#define MBEDTLS_PSA_BUILTIN_ALG_STREAM_CIPHER 1
#endif /* MBEDTLS_PSA_ACCEL_ALG_STREAM_CIPHER */
#endif /* PSA_WANT_ALG_STREAM_CIPHER */

#if defined(PSA_WANT_KEY_TYPE_CHACHA20)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_CHACHA20) || \
    defined(MBEDTLS_PSA_BUILTIN_ALG_STREAM_CIPHER)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_CHACHA20 1
#define MBEDTLS_CHACHA20_C
#endif /*!MBEDTLS_PSA_ACCEL_KEY_TYPE_CHACHA20 */
#endif /* PSA_WANT_KEY_TYPE_CHACHA20 */

/* If any of the software block ciphers are selected, define
 * PSA_HAVE_SOFT_BLOCK_CIPHER, which can be used in any of these
 * situations. */
#if defined(PSA_HAVE_SOFT_KEY_TYPE_AES) || \
    defined(PSA_HAVE_SOFT_KEY_TYPE_ARIA) || \
    defined(PSA_HAVE_SOFT_KEY_TYPE_DES) || \
    defined(PSA_HAVE_SOFT_KEY_TYPE_CAMELLIA)
#define PSA_HAVE_SOFT_BLOCK_CIPHER 1
#endif

#if defined(PSA_WANT_ALG_CBC_MAC)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_CBC_MAC)
#error "CBC-MAC is not yet supported via the PSA API in Mbed TLS."
#define MBEDTLS_PSA_BUILTIN_ALG_CBC_MAC 1
#endif /* !MBEDTLS_PSA_ACCEL_ALG_CBC_MAC */
#endif /* PSA_WANT_ALG_CBC_MAC */

#if defined(PSA_WANT_ALG_CMAC)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_CMAC) || \
    defined(PSA_HAVE_SOFT_BLOCK_CIPHER)
#define MBEDTLS_PSA_BUILTIN_ALG_CMAC 1
#define MBEDTLS_CMAC_C
#endif /* !MBEDTLS_PSA_ACCEL_ALG_CMAC */
#endif /* PSA_WANT_ALG_CMAC */

#if defined(PSA_HAVE_SOFT_PBKDF2_HMAC) || \
    defined(PSA_HAVE_SOFT_PBKDF2_CMAC)
#define PSA_HAVE_SOFT_PBKDF2 1
#endif /* PSA_HAVE_SOFT_PBKDF2_HMAC || PSA_HAVE_SOFT_PBKDF2_CMAC */

#if defined(PSA_WANT_ALG_CTR)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_CTR) || \
    defined(PSA_HAVE_SOFT_BLOCK_CIPHER)
#define MBEDTLS_PSA_BUILTIN_ALG_CTR 1
#define MBEDTLS_CIPHER_MODE_CTR
#endif
#endif /* PSA_WANT_ALG_CTR */

#if defined(PSA_WANT_ALG_CFB)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_CFB) || \
    defined(PSA_HAVE_SOFT_BLOCK_CIPHER)
#define MBEDTLS_PSA_BUILTIN_ALG_CFB 1
#define MBEDTLS_CIPHER_MODE_CFB
#endif
#endif /* PSA_WANT_ALG_CFB */

#if defined(PSA_WANT_ALG_OFB)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_OFB) || \
    defined(PSA_HAVE_SOFT_BLOCK_CIPHER)
#define MBEDTLS_PSA_BUILTIN_ALG_OFB 1
#define MBEDTLS_CIPHER_MODE_OFB
#endif
#endif /* PSA_WANT_ALG_OFB */

#if defined(PSA_WANT_ALG_ECB_NO_PADDING) &&     \
    !defined(MBEDTLS_PSA_ACCEL_ALG_ECB_NO_PADDING)
#define MBEDTLS_PSA_BUILTIN_ALG_ECB_NO_PADDING 1
#endif

#if defined(PSA_WANT_ALG_CBC_NO_PADDING)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_CBC_NO_PADDING) || \
    defined(PSA_HAVE_SOFT_BLOCK_CIPHER)
#define MBEDTLS_CIPHER_MODE_CBC
#define MBEDTLS_PSA_BUILTIN_ALG_CBC_NO_PADDING 1
#endif
#endif /* PSA_WANT_ALG_CBC_NO_PADDING */

#if defined(PSA_WANT_ALG_CBC_PKCS7)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_CBC_PKCS7) || \
    defined(PSA_HAVE_SOFT_BLOCK_CIPHER)
#define MBEDTLS_CIPHER_MODE_CBC
#define MBEDTLS_PSA_BUILTIN_ALG_CBC_PKCS7 1
#define MBEDTLS_CIPHER_PADDING_PKCS7
#endif
#endif /* PSA_WANT_ALG_CBC_PKCS7 */

#if defined(PSA_WANT_ALG_CCM)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_CCM) || \
    defined(PSA_HAVE_SOFT_KEY_TYPE_AES) || \
    defined(PSA_HAVE_SOFT_KEY_TYPE_ARIA) || \
    defined(PSA_HAVE_SOFT_KEY_TYPE_CAMELLIA)
#define MBEDTLS_PSA_BUILTIN_ALG_CCM 1
#define MBEDTLS_CCM_C
#endif
#endif /* PSA_WANT_ALG_CCM */

#if defined(PSA_WANT_ALG_CCM_STAR_NO_TAG)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_CCM_STAR_NO_TAG) || \
    defined(PSA_HAVE_SOFT_KEY_TYPE_AES) || \
    defined(PSA_HAVE_SOFT_KEY_TYPE_ARIA) || \
    defined(PSA_HAVE_SOFT_KEY_TYPE_CAMELLIA)
#define MBEDTLS_PSA_BUILTIN_ALG_CCM_STAR_NO_TAG 1
#define MBEDTLS_CCM_C
#endif
#endif /* PSA_WANT_ALG_CCM_STAR_NO_TAG */

#if defined(PSA_WANT_ALG_GCM)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_GCM) || \
    defined(PSA_HAVE_SOFT_KEY_TYPE_AES) || \
    defined(PSA_HAVE_SOFT_KEY_TYPE_ARIA) || \
    defined(PSA_HAVE_SOFT_KEY_TYPE_CAMELLIA)
#define MBEDTLS_PSA_BUILTIN_ALG_GCM 1
#define MBEDTLS_GCM_C
#endif
#endif /* PSA_WANT_ALG_GCM */

#if defined(PSA_WANT_ALG_CHACHA20_POLY1305)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_CHACHA20_POLY1305)
#if defined(PSA_WANT_KEY_TYPE_CHACHA20)
#define MBEDTLS_CHACHAPOLY_C
#define MBEDTLS_CHACHA20_C
#define MBEDTLS_POLY1305_C
#define MBEDTLS_PSA_BUILTIN_ALG_CHACHA20_POLY1305 1
#endif /* PSA_WANT_KEY_TYPE_CHACHA20 */
#endif /* !MBEDTLS_PSA_ACCEL_ALG_CHACHA20_POLY1305 */
#endif /* PSA_WANT_ALG_CHACHA20_POLY1305 */

#endif /* MBEDTLS_CONFIG_ADJUST_LEGACY_FROM_PSA_H */