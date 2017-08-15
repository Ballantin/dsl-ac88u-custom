/*
 *  LZO1X Compressor from MiniLZO
 *
 *  Copyright (C) 1996-2005 Markus F.X.J. Oberhumer <markus@oberhumer.com>
 *
 *  The full LZO package can be found at:
 *  http://www.oberhumer.com/opensource/lzo/
 *
 *  Changed for kernel use by:
 *  Nitin Gupta <nitingupta910@gmail.com>
 *  Richard Purdie <rpurdie@openedhand.com>
 */

#include <linux/module.h>
#include <linux/kernel.h>
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
#include <linux/lzo.h>
#endif
#include <asm/unaligned.h>
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
#include <linux/lzo.h>
#endif
#include "lzodefs.h"

static noinline size_t
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
_lzo1x_1_do_compress(const unsigned char *in, size_t in_len,
		unsigned char *out, size_t *out_len, void *wrkmem)
#else
lzo1x_1_do_compress(const unsigned char *in, size_t in_len,
		    unsigned char *out, size_t *out_len,
		    size_t ti, void *wrkmem)
#endif
{
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
	const unsigned char *ip;
	unsigned char *op;
#endif
	const unsigned char * const in_end = in + in_len;
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	const unsigned char * const ip_end = in + in_len - M2_MAX_LEN - 5;
	const unsigned char ** const dict = wrkmem;
	const unsigned char *ip = in, *ii = ip;
	const unsigned char *end, *m, *m_pos;
	size_t m_off, m_len, dindex;
	unsigned char *op = out;
#else
	const unsigned char * const ip_end = in + in_len - 20;
	const unsigned char *ii;
	lzo_dict_t * const dict = (lzo_dict_t *) wrkmem;
#endif

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	ip += 4;
#else
	op = out;
	ip = in;
	ii = ip;
	ip += ti < 4 ? 4 - ti : 0;
#endif

	for (;;) {
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
		dindex = ((size_t)(0x21 * DX3(ip, 5, 5, 6)) >> 5) & D_MASK;
		m_pos = dict[dindex];

		if (m_pos < in)
			goto literal;

		if (ip == m_pos || ((size_t)(ip - m_pos) > M4_MAX_OFFSET))
			goto literal;

		m_off = ip - m_pos;
		if (m_off <= M2_MAX_OFFSET || m_pos[3] == ip[3])
			goto try_match;

		dindex = (dindex & (D_MASK & 0x7ff)) ^ (D_HIGH | 0x1f);
		m_pos = dict[dindex];

		if (m_pos < in)
			goto literal;

		if (ip == m_pos || ((size_t)(ip - m_pos) > M4_MAX_OFFSET))
			goto literal;

		m_off = ip - m_pos;
		if (m_off <= M2_MAX_OFFSET || m_pos[3] == ip[3])
			goto try_match;

		goto literal;

try_match:
		if (get_unaligned((const unsigned short *)m_pos)
				== get_unaligned((const unsigned short *)ip)) {
			if (likely(m_pos[2] == ip[2]))
					goto match;
		}

#else
		const unsigned char *m_pos;
		size_t t, m_len, m_off;
		u32 dv;
#endif
literal:
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
		dict[dindex] = ip;
		++ip;
#else
		ip += 1 + ((ip - ii) >> 5);
next:
#endif
		if (unlikely(ip >= ip_end))
			break;
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
		continue;

match:
		dict[dindex] = ip;
		if (ip != ii) {
			size_t t = ip - ii;
#else
		dv = get_unaligned_le32(ip);
		t = ((dv * 0x1824429d) >> (32 - D_BITS)) & D_MASK;
		m_pos = in + dict[t];
		dict[t] = (lzo_dict_t) (ip - in);
		if (unlikely(dv != get_unaligned_le32(m_pos)))
			goto literal;
#endif

#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
		ii -= ti;
		ti = 0;
		t = ip - ii;
		if (t != 0) {
#endif
			if (t <= 3) {
				op[-2] |= t;
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
			} else if (t <= 18) {
#else
				COPY4(op, ii);
				op += t;
			} else if (t <= 16) {
#endif
				*op++ = (t - 3);
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
				COPY8(op, ii);
				COPY8(op + 8, ii + 8);
				op += t;
#endif
			} else {
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
				size_t tt = t - 18;

				*op++ = 0;
				while (tt > 255) {
					tt -= 255;
#else
				if (t <= 18) {
					*op++ = (t - 3);
				} else {
					size_t tt = t - 18;
#endif
					*op++ = 0;
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
					while (unlikely(tt > 255)) {
						tt -= 255;
						*op++ = 0;
					}
					*op++ = tt;
#endif
				}
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
				*op++ = tt;
#else
				do {
					COPY8(op, ii);
					COPY8(op + 8, ii + 8);
					op += 16;
					ii += 16;
					t -= 16;
				} while (t >= 16);
				if (t > 0) do {
					*op++ = *ii++;
				} while (--t > 0);
#endif
			}
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
			do {
				*op++ = *ii++;
			} while (--t > 0);
#endif
		}

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
		ip += 3;
		if (m_pos[3] != *ip++ || m_pos[4] != *ip++
				|| m_pos[5] != *ip++ || m_pos[6] != *ip++
				|| m_pos[7] != *ip++ || m_pos[8] != *ip++) {
			--ip;
			m_len = ip - ii;
#else
		m_len = 4;
		{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS) && defined(LZO_USE_CTZ64)
		u64 v;
		v = get_unaligned((const u64 *) (ip + m_len)) ^
		    get_unaligned((const u64 *) (m_pos + m_len));
		if (unlikely(v == 0)) {
			do {
				m_len += 8;
				v = get_unaligned((const u64 *) (ip + m_len)) ^
				    get_unaligned((const u64 *) (m_pos + m_len));
				if (unlikely(ip + m_len >= ip_end))
					goto m_len_done;
			} while (v == 0);
		}
#  if defined(__LITTLE_ENDIAN)
		m_len += (unsigned) __builtin_ctzll(v) / 8;
#  elif defined(__BIG_ENDIAN)
		m_len += (unsigned) __builtin_clzll(v) / 8;
#  else
#    error "missing endian definition"
#  endif
#elif defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS) && defined(LZO_USE_CTZ32)
		u32 v;
		v = get_unaligned((const u32 *) (ip + m_len)) ^
		    get_unaligned((const u32 *) (m_pos + m_len));
		if (unlikely(v == 0)) {
			do {
				m_len += 4;
				v = get_unaligned((const u32 *) (ip + m_len)) ^
				    get_unaligned((const u32 *) (m_pos + m_len));
				if (v != 0)
					break;
				m_len += 4;
				v = get_unaligned((const u32 *) (ip + m_len)) ^
				    get_unaligned((const u32 *) (m_pos + m_len));
				if (unlikely(ip + m_len >= ip_end))
					goto m_len_done;
			} while (v == 0);
		}
#  if defined(__LITTLE_ENDIAN)
		m_len += (unsigned) __builtin_ctz(v) / 8;
#  elif defined(__BIG_ENDIAN)
		m_len += (unsigned) __builtin_clz(v) / 8;
#  else
#    error "missing endian definition"
#  endif
#else
		if (unlikely(ip[m_len] == m_pos[m_len])) {
			do {
				m_len += 1;
				if (ip[m_len] != m_pos[m_len])
					break;
				m_len += 1;
				if (ip[m_len] != m_pos[m_len])
					break;
				m_len += 1;
				if (ip[m_len] != m_pos[m_len])
					break;
				m_len += 1;
				if (ip[m_len] != m_pos[m_len])
					break;
				m_len += 1;
				if (ip[m_len] != m_pos[m_len])
					break;
				m_len += 1;
				if (ip[m_len] != m_pos[m_len])
					break;
				m_len += 1;
				if (ip[m_len] != m_pos[m_len])
					break;
				m_len += 1;
				if (unlikely(ip + m_len >= ip_end))
					goto m_len_done;
			} while (ip[m_len] == m_pos[m_len]);
		}
#endif
		}
m_len_done:
#endif

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
			if (m_off <= M2_MAX_OFFSET) {
				m_off -= 1;
				*op++ = (((m_len - 1) << 5)
						| ((m_off & 7) << 2));
				*op++ = (m_off >> 3);
			} else if (m_off <= M3_MAX_OFFSET) {
				m_off -= 1;
#else
		m_off = ip - m_pos;
		ip += m_len;
		ii = ip;
		if (m_len <= M2_MAX_LEN && m_off <= M2_MAX_OFFSET) {
			m_off -= 1;
			*op++ = (((m_len - 1) << 5) | ((m_off & 7) << 2));
			*op++ = (m_off >> 3);
		} else if (m_off <= M3_MAX_OFFSET) {
			m_off -= 1;
			if (m_len <= M3_MAX_LEN)
#endif
				*op++ = (M3_MARKER | (m_len - 2));
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
				goto m3_m4_offset;
			} else {
				m_off -= 0x4000;

				*op++ = (M4_MARKER | ((m_off & 0x4000) >> 11)
						| (m_len - 2));
				goto m3_m4_offset;
#else
			else {
				m_len -= M3_MAX_LEN;
				*op++ = M3_MARKER | 0;
				while (unlikely(m_len > 255)) {
					m_len -= 255;
					*op++ = 0;
				}
				*op++ = (m_len);
#endif
			}
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
			*op++ = (m_off << 2);
			*op++ = (m_off >> 6);
#endif
		} else {
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
			end = in_end;
			m = m_pos + M2_MAX_LEN + 1;

			while (ip < end && *m == *ip) {
				m++;
				ip++;
			}
			m_len = ip - ii;

			if (m_off <= M3_MAX_OFFSET) {
				m_off -= 1;
				if (m_len <= 33) {
					*op++ = (M3_MARKER | (m_len - 2));
				} else {
					m_len -= 33;
					*op++ = M3_MARKER | 0;
					goto m3_m4_len;
				}
			} else {
				m_off -= 0x4000;
				if (m_len <= M4_MAX_LEN) {
					*op++ = (M4_MARKER
						| ((m_off & 0x4000) >> 11)
#else
			m_off -= 0x4000;
			if (m_len <= M4_MAX_LEN)
				*op++ = (M4_MARKER | ((m_off >> 11) & 8)
#endif
						| (m_len - 2));
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
				} else {
					m_len -= M4_MAX_LEN;
					*op++ = (M4_MARKER
						| ((m_off & 0x4000) >> 11));
m3_m4_len:
					while (m_len > 255) {
						m_len -= 255;
						*op++ = 0;
					}

					*op++ = (m_len);
#else
			else {
				m_len -= M4_MAX_LEN;
				*op++ = (M4_MARKER | ((m_off >> 11) & 8));
				while (unlikely(m_len > 255)) {
					m_len -= 255;
					*op++ = 0;
#endif
				}
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
				*op++ = (m_len);
#endif
			}
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
m3_m4_offset:
			*op++ = ((m_off & 63) << 2);
#else
			*op++ = (m_off << 2);
#endif
			*op++ = (m_off >> 6);
		}
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)

		ii = ip;
		if (unlikely(ip >= ip_end))
			break;
#else
		goto next;
#endif
	}
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)

#endif
	*out_len = op - out;
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	return in_end - ii;
#else
	return in_end - (ii - ti);
#endif
}

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
int lzo1x_1_compress(const unsigned char *in, size_t in_len, unsigned char *out,
			size_t *out_len, void *wrkmem)
#else
int lzo1x_1_compress(const unsigned char *in, size_t in_len,
		     unsigned char *out, size_t *out_len,
		     void *wrkmem)
#endif
{
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	const unsigned char *ii;
#else
	const unsigned char *ip = in;
#endif
	unsigned char *op = out;
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	size_t t;
#else
	size_t l = in_len;
	size_t t = 0;
#endif

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	if (unlikely(in_len <= M2_MAX_LEN + 5)) {
		t = in_len;
	} else {
		t = _lzo1x_1_do_compress(in, in_len, op, out_len, wrkmem);
#else
	while (l > 20) {
		size_t ll = l <= (M4_MAX_OFFSET + 1) ? l : (M4_MAX_OFFSET + 1);
		uintptr_t ll_end = (uintptr_t) ip + ll;
		if ((ll_end + ((t + ll) >> 5)) <= ll_end)
			break;
		BUILD_BUG_ON(D_SIZE * sizeof(lzo_dict_t) > LZO1X_1_MEM_COMPRESS);
		memset(wrkmem, 0, D_SIZE * sizeof(lzo_dict_t));
		t = lzo1x_1_do_compress(ip, ll, op, out_len, t, wrkmem);
		ip += ll;
#endif
		op += *out_len;
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
		l  -= ll;
#endif
	}
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
	t += l;
#endif

	if (t > 0) {
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
		ii = in + in_len - t;
#else
		const unsigned char *ii = in + in_len - t;
#endif

		if (op == out && t <= 238) {
			*op++ = (17 + t);
		} else if (t <= 3) {
			op[-2] |= t;
		} else if (t <= 18) {
			*op++ = (t - 3);
		} else {
			size_t tt = t - 18;
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)

#endif
			*op++ = 0;
			while (tt > 255) {
				tt -= 255;
				*op++ = 0;
			}
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)

#endif
			*op++ = tt;
		}
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
		do {
#else
		if (t >= 16) do {
			COPY8(op, ii);
			COPY8(op + 8, ii + 8);
			op += 16;
			ii += 16;
			t -= 16;
		} while (t >= 16);
		if (t > 0) do {
#endif
			*op++ = *ii++;
		} while (--t > 0);
	}

	*op++ = M4_MARKER | 1;
	*op++ = 0;
	*op++ = 0;

	*out_len = op - out;
	return LZO_E_OK;
}
EXPORT_SYMBOL_GPL(lzo1x_1_compress);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LZO1X-1 Compressor");
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)

#endif
