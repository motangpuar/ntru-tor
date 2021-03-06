/* Copyright (c) 2012-2013, The Tor Project, Inc. */
/* See LICENSE for licensing information */

#ifndef TOR_ONION_NTOR_NTRU_H
#define TOR_ONION_NTOR_NTRU_H

#include "torint.h"
#include "crypto_curve25519.h"
#include "crypto_ntruees439ep1.h"
#include "di_ops.h"

/** State to be maintained by a client between sending an ntor+ntru
 * onionskin and receiving a reply. */
typedef struct ntor_ntru_handshake_state_t ntor_ntru_handshake_state_t;

/** Length of an ntor onionskin, as sent from the client to server. */
#define NTOR_NTRU_ONIONSKIN_LEN 693
/** Length of an ntor reply, as sent from server to client. */
#define NTOR_NTRU_REPLY_LEN 673
/** Length of encapsulated key (s2) contained in NTRU ciphertext */
#define NTOR_NTRU_ENCAPSULATED_KEY_LEN 32

#ifdef NTOR_NTRU_ENABLED
void ntor_ntru_handshake_state_free(ntor_ntru_handshake_state_t *state);

int onion_skin_ntor_ntru_create(const uint8_t *router_id,
                                const curve25519_public_key_t *router_key,
                                ntor_ntru_handshake_state_t **handshake_state_out,
                                uint8_t *onion_skin_out);

int onion_skin_ntor_ntru_server_handshake(const uint8_t *onion_skin,
                                 const di_digest256_map_t *private_keys,
                                 const curve25519_keypair_t *junk_keypair,
                                 const uint8_t *my_node_id,
                                 uint8_t *handshake_reply_out,
                                 uint8_t *key_out,
                                 size_t key_out_len);

int onion_skin_ntor_ntru_client_handshake(
                             const ntor_ntru_handshake_state_t *handshake_state,
                             const uint8_t *handshake_reply,
                             uint8_t *key_out,
                             size_t key_out_len);

#ifdef ONION_NTOR_NTRU_PRIVATE

/** Storage held by a client while waiting for an ntor reply from a server. */
struct ntor_ntru_handshake_state_t {
  /** Identity digest of the router we're talking to. */
  uint8_t router_id[DIGEST_LEN];
  /** Onion key of the router we're talking to. */
  curve25519_public_key_t pubkey_B;

  /**
   * Short-lived ECDH keypair for use with this handshake.
   * @{ */
  curve25519_secret_key_t seckey_x;
  curve25519_public_key_t pubkey_X;
  /** @} */

  /**
   * Short-lived NTRU keypair for use with this handshake.
   * @{ */
  ntruees439ep1_secret_key_t seckey_n;
  ntruees439ep1_public_key_t pubkey_N;
  /** @} */
};
#endif

#endif

#endif

