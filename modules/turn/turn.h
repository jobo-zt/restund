/**
 * @file turn.h Internal TURN interface
 *
 * Copyright (C) 2010 Creytiv.com
 */

struct turnd {
	struct sa rel_addr;
	struct sa rel_addr6;
	struct sa public_addr;
	struct hash *ht_alloc;
	uint64_t bytec_tx;
	uint64_t bytec_rx;
	uint64_t errc_tx;
	uint64_t errc_rx;
	uint64_t allocc_tot;
	uint32_t allocc_cur;
	uint32_t lifetime_max;
	uint32_t udp_sockbuf_size;
	mtx_t mutex;
	bool run;
	struct list re_map;
	struct list rm_map;
	uint64_t ts;

	struct {
		uint64_t scode_400;
		uint64_t scode_420;
		uint64_t scode_437;
		uint64_t scode_440;
		uint64_t scode_441;
		uint64_t scode_442;
		uint64_t scode_443;
		uint64_t scode_500;
		uint64_t scode_508;
	} reply;
};

struct chanlist;

struct udp_socks{
	struct le le;
	thrd_t thrd;
	struct udp_sock *rel_us;
	struct udp_sock *rsv_us;
};

struct allocation {
	struct le he;
	mtx_t mutex;
	struct tmr tmr;
	uint8_t tid[STUN_TID_SIZE];
	struct sa cli_addr;
	struct sa srv_addr;
	struct sa rel_addr;
	struct sa rsv_addr;
	void *cli_sock;
	struct udp_socks *uks;
	char *username;
	struct hash *perms;
	struct chanlist *chans;
	uint64_t dropc_tx;
	uint64_t dropc_rx;
	int proto;
};

void allocate_request(struct turnd *turnd, struct allocation *alx,
		      struct restund_msgctx *ctx, int proto, void *sock,
		      const struct sa *src, const struct sa *dst,
		      const struct stun_msg *msg);
void refresh_request(struct turnd *turnd, struct allocation *al,
		     struct restund_msgctx *ctx,
		     int proto, void *sock, const struct sa *src,
		     const struct stun_msg *msg);
void createperm_request(struct allocation *al, struct restund_msgctx *ctx,
			int proto, void *sock, const struct sa *src,
			const struct stun_msg *msg);
void chanbind_request(struct allocation *al, struct restund_msgctx *ctx,
		      int proto, void *sock, const struct sa *src,
		      const struct stun_msg *msg);
struct turnd *turndp(void);


struct perm;

struct perm *perm_find(const struct hash *ht, const struct sa *addr);
struct perm *perm_create(struct hash *ht, const struct sa *peer,
			 struct allocation *al);
void perm_refresh(struct perm *perm);
void perm_tx_stat(struct perm *perm, size_t bytc);
void perm_rx_stat(struct perm *perm, size_t bytc);
int  perm_hash_alloc(struct hash **ht, uint32_t bsize);
void perm_status(struct hash *ht, struct mbuf *mb);


struct chan;

struct chan *chan_numb_find(const struct chanlist *cl, uint16_t numb);
struct chan *chan_peer_find(const struct chanlist *cl, const struct sa *peer);
uint16_t chan_numb(const struct chan *chan);
const struct sa *chan_peer(const struct chan *chan);
int  chanlist_alloc(struct chanlist **clp, uint32_t bsize);
void chan_status(const struct chanlist *cl, struct mbuf *mb);
