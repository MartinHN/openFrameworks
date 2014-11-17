#include "fitz-internal.h"

typedef unsigned char byte;

//static inline float roundup(float x)
//{
//	return (x < 0) ? floorf(x) : ceilf(x);
//}

static inline int lerp(int a, int b, int t)
{
	return a + (((b - a) * t) >> 16);
}

static inline int bilerp(int a, int b, int c, int d, int u, int v)
{
	return lerp(lerp(a, b, u), lerp(c, d, u), v);
}

static inline byte *sample_nearest(byte *s, int w, int h, int n, int u, int v)
{
	if (u < 0) u = 0;
	if (v < 0) v = 0;
	if (u >= w) u = w - 1;
	if (v >= h) v = h - 1;
	return s + (v * w + u) * n;
}

/* Blend premultiplied source image in constant alpha over destination */

static inline void
fz_paint_affine_alpha_N_lerp(byte *dp, byte *sp, int sw, int sh, int u, int v, int fa, int fb, int w, int n, int alpha, byte *hp)
{
	int k;
	int n1 = n-1;

	while (w--)
	{
		int ui = u >> 16;
		int vi = v >> 16;
		if (ui >= 0 && ui < sw && vi >= 0 && vi < sh)
		{
			int uf = u & 0xffff;
			int vf = v & 0xffff;
			byte *a = sample_nearest(sp, sw, sh, n, ui, vi);
			byte *b = sample_nearest(sp, sw, sh, n, ui+1, vi);
			byte *c = sample_nearest(sp, sw, sh, n, ui, vi+1);
			byte *d = sample_nearest(sp, sw, sh, n, ui+1, vi+1);
			int xa = bilerp(a[n1], b[n1], c[n1], d[n1], uf, vf);
			int t;
			xa = fz_mul255(xa, alpha);
			t = 255 - xa;
			for (k = 0; k < n1; k++)
			{
				int x = bilerp(a[k], b[k], c[k], d[k], uf, vf);
				dp[k] = fz_mul255(x, alpha) + fz_mul255(dp[k], t);
			}
			dp[n1] = xa + fz_mul255(dp[n1], t);
			if (hp)
				hp[0] = xa + fz_mul255(hp[0], t);
		}
		dp += n;
		if (hp)
			hp++;
		u += fa;
		v += fb;
	}
}

/* Special case code for gray -> rgb */
static inline void
fz_paint_affine_alpha_g2rgb_lerp(byte *dp, byte *sp, int sw, int sh, int u, int v, int fa, int fb, int w, int alpha, byte *hp)
{
	while (w--)
	{
		int ui = u >> 16;
		int vi = v >> 16;
		if (ui >= 0 && ui < sw && vi >= 0 && vi < sh)
		{
			int uf = u & 0xffff;
			int vf = v & 0xffff;
			byte *a = sample_nearest(sp, sw, sh, 2, ui, vi);
			byte *b = sample_nearest(sp, sw, sh, 2, ui+1, vi);
			byte *c = sample_nearest(sp, sw, sh, 2, ui, vi+1);
			byte *d = sample_nearest(sp, sw, sh, 2, ui+1, vi+1);
			int y = bilerp(a[1], b[1], c[1], d[1], uf, vf);
			int x = bilerp(a[0], b[0], c[0], d[0], uf, vf);
			int t;
			x = fz_mul255(x, alpha);
			y = fz_mul255(y, alpha);
			t = 255 - y;
			dp[0] = x + fz_mul255(dp[0], t);
			dp[1] = x + fz_mul255(dp[1], t);
			dp[2] = x + fz_mul255(dp[2], t);
			dp[3] = y + fz_mul255(dp[3], t);
			if (hp)
				hp[0] = y + fz_mul255(hp[0], t);
		}
		dp += 4;
		if (hp)
			hp++;
		u += fa;
		v += fb;
	}
}

static inline void
fz_paint_affine_alpha_N_near(byte *dp, byte *sp, int sw, int sh, int u, int v, int fa, int fb, int w, int n, int alpha, byte *hp)
{
	int k;
	int n1 = n-1;

	while (w--)
	{
		int ui = u >> 16;
		int vi = v >> 16;
		if (ui >= 0 && ui < sw && vi >= 0 && vi < sh)
		{
			byte *sample = sp + ((vi * sw + ui) * n);
			int a = fz_mul255(sample[n-1], alpha);
			int t = 255 - a;
			for (k = 0; k < n1; k++)
				dp[k] = fz_mul255(sample[k], alpha) + fz_mul255(dp[k], t);
			dp[n1] = a + fz_mul255(dp[n1], t);
			if (hp)
				hp[0] = a + fz_mul255(hp[0], t);
		}
		dp += n;
		if (hp)
			hp++;
		u += fa;
		v += fb;
	}
}

static inline void
fz_paint_affine_alpha_g2rgb_near(byte *dp, byte *sp, int sw, int sh, int u, int v, int fa, int fb, int w, int alpha, byte *hp)
{
	while (w--)
	{
		int ui = u >> 16;
		int vi = v >> 16;
		if (ui >= 0 && ui < sw && vi >= 0 && vi < sh)
		{
			byte *sample = sp + ((vi * sw + ui) * 2);
			int x = fz_mul255(sample[0], alpha);
			int a = fz_mul255(sample[1], alpha);
			int t = 255 - a;
			dp[0] = x + fz_mul255(dp[0], t);
			dp[1] = x + fz_mul255(dp[1], t);
			dp[2] = x + fz_mul255(dp[2], t);
			dp[3] = a + fz_mul255(dp[3], t);
			if (hp)
				hp[0] = a + fz_mul255(hp[0], t);
		}
		dp += 4;
		if (hp)
			hp++;
		u += fa;
		v += fb;
	}
}

/* Blend premultiplied source image over destination */

static inline void
fz_paint_affine_N_lerp(byte *dp, byte *sp, int sw, int sh, int u, int v, int fa, int fb, int w, int n, byte *hp)
{
	int k;
	int n1 = n-1;

	while (w--)
	{
		int ui = u >> 16;
		int vi = v >> 16;
		if (ui >= 0 && ui < sw && vi >= 0 && vi < sh)
		{
			int uf = u & 0xffff;
			int vf = v & 0xffff;
			byte *a = sample_nearest(sp, sw, sh, n, ui, vi);
			byte *b = sample_nearest(sp, sw, sh, n, ui+1, vi);
			byte *c = sample_nearest(sp, sw, sh, n, ui, vi+1);
			byte *d = sample_nearest(sp, sw, sh, n, ui+1, vi+1);
			int y = bilerp(a[n1], b[n1], c[n1], d[n1], uf, vf);
			int t = 255 - y;
			for (k = 0; k < n1; k++)
			{
				int x = bilerp(a[k], b[k], c[k], d[k], uf, vf);
				dp[k] = x + fz_mul255(dp[k], t);
			}
			dp[n1] = y + fz_mul255(dp[n1], t);
			if (hp)
				hp[0] = y + fz_mul255(hp[0], t);
		}
		dp += n;
		if (hp)
			hp++;
		u += fa;
		v += fb;
	}
}

static inline void
fz_paint_affine_solid_g2rgb_lerp(byte *dp, byte *sp, int sw, int sh, int u, int v, int fa, int fb, int w, byte *hp)
{
	while (w--)
	{
		int ui = u >> 16;
		int vi = v >> 16;
		if (ui >= 0 && ui < sw && vi >= 0 && vi < sh)
		{
			int uf = u & 0xffff;
			int vf = v & 0xffff;
			byte *a = sample_nearest(sp, sw, sh, 2, ui, vi);
			byte *b = sample_nearest(sp, sw, sh, 2, ui+1, vi);
			byte *c = sample_nearest(sp, sw, sh, 2, ui, vi+1);
			byte *d = sample_nearest(sp, sw, sh, 2, ui+1, vi+1);
			int y = bilerp(a[1], b[1], c[1], d[1], uf, vf);
			int t = 255 - y;
			int x = bilerp(a[0], b[0], c[0], d[0], uf, vf);
			dp[0] = x + fz_mul255(dp[0], t);
			dp[1] = x + fz_mul255(dp[1], t);
			dp[2] = x + fz_mul255(dp[2], t);
			dp[3] = y + fz_mul255(dp[3], t);
			if (hp)
				hp[0] = y + fz_mul255(hp[0], t);
		}
		dp += 4;
		if (hp)
			hp++;
		u += fa;
		v += fb;
	}
}

static inline void
fz_paint_affine_N_near(byte *dp, byte *sp, int sw, int sh, int u, int v, int fa, int fb, int w, int n, byte *hp)
{
	int k;
	int n1 = n-1;

	while (w--)
	{
		int ui = u >> 16;
		int vi = v >> 16;
		if (ui >= 0 && ui < sw && vi >= 0 && vi < sh)
		{
			byte *sample = sp + ((vi * sw + ui) * n);
			int a = sample[n1];
			int t = 255 - a;
			for (k = 0; k < n1; k++)
				dp[k] = sample[k] + fz_mul255(dp[k], t);
			dp[n1] = a + fz_mul255(dp[n1], t);
			if (hp)
				hp[0] = a + fz_mul255(hp[0], t);
		}
		dp += n;
		if (hp)
			hp++;
		u += fa;
		v += fb;
	}
}

static inline void
fz_paint_affine_solid_g2rgb_near(byte *dp, byte *sp, int sw, int sh, int u, int v, int fa, int fb, int w, byte *hp)
{
	while (w--)
	{
		int ui = u >> 16;
		int vi = v >> 16;
		if (ui >= 0 && ui < sw && vi >= 0 && vi < sh)
		{
			byte *sample = sp + ((vi * sw + ui) * 2);
			int x = sample[0];
			int a = sample[1];
			int t = 255 - a;
			dp[0] = x + fz_mul255(dp[0], t);
			dp[1] = x + fz_mul255(dp[1], t);
			dp[2] = x + fz_mul255(dp[2], t);
			dp[3] = a + fz_mul255(dp[3], t);
			if (hp)
				hp[0] = a + fz_mul255(hp[0], t);
		}
		dp += 4;
		if (hp)
			hp++;
		u += fa;
		v += fb;
	}
}

/* Blend non-premultiplied color in source image mask over destination */

static inline void
fz_paint_affine_color_N_lerp(byte *dp, byte *sp, int sw, int sh, int u, int v, int fa, int fb, int w, int n, byte *color, byte *hp)
{
	int n1 = n - 1;
	int sa = color[n1];
	int k;

	while (w--)
	{
		int ui = u >> 16;
		int vi = v >> 16;
		if (ui >= 0 && ui < sw && vi >= 0 && vi < sh)
		{
			int uf = u & 0xffff;
			int vf = v & 0xffff;
			byte *a = sample_nearest(sp, sw, sh, 1, ui, vi);
			byte *b = sample_nearest(sp, sw, sh, 1, ui+1, vi);
			byte *c = sample_nearest(sp, sw, sh, 1, ui, vi+1);
			byte *d = sample_nearest(sp, sw, sh, 1, ui+1, vi+1);
			int ma = bilerp(a[0], b[0], c[0], d[0], uf, vf);
			int masa = FZ_COMBINE(FZ_EXPAND(ma), sa);
			for (k = 0; k < n1; k++)
				dp[k] = FZ_BLEND(color[k], dp[k], masa);
			dp[n1] = FZ_BLEND(255, dp[n1], masa);
			if (hp)
				hp[0] = FZ_BLEND(255, hp[0], masa);
		}
		dp += n;
		if (hp)
			hp++;
		u += fa;
		v += fb;
	}
}

static inline void
fz_paint_affine_color_N_near(byte *dp, byte *sp, int sw, int sh, int u, int v, int fa, int fb, int w, int n, byte *color, byte *hp)
{
	int n1 = n-1;
	int sa = color[n1];
	int k;

	while (w--)
	{
		int ui = u >> 16;
		int vi = v >> 16;
		if (ui >= 0 && ui < sw && vi >= 0 && vi < sh)
		{
			int ma = sp[vi * sw + ui];
			int masa = FZ_COMBINE(FZ_EXPAND(ma), sa);
			for (k = 0; k < n1; k++)
				dp[k] = FZ_BLEND(color[k], dp[k], masa);
			dp[n1] = FZ_BLEND(255, dp[n1], masa);
			if (hp)
				hp[0] = FZ_BLEND(255, hp[0], masa);
		}
		dp += n;
		if (hp)
			hp++;
		u += fa;
		v += fb;
	}
}

static void
fz_paint_affine_lerp(byte *dp, byte *sp, int sw, int sh, int u, int v, int fa, int fb, int w, int n, int alpha, byte *color/*unused*/, byte *hp)
{
	if (alpha == 255)
	{
		switch (n)
		{
		case 1: fz_paint_affine_N_lerp(dp, sp, sw, sh, u, v, fa, fb, w, 1, hp); break;
		case 2: fz_paint_affine_N_lerp(dp, sp, sw, sh, u, v, fa, fb, w, 2, hp); break;
		case 4: fz_paint_affine_N_lerp(dp, sp, sw, sh, u, v, fa, fb, w, 4, hp); break;
		default: fz_paint_affine_N_lerp(dp, sp, sw, sh, u, v, fa, fb, w, n, hp); break;
		}
	}
	else if (alpha > 0)
	{
		switch (n)
		{
		case 1: fz_paint_affine_alpha_N_lerp(dp, sp, sw, sh, u, v, fa, fb, w, 1, alpha, hp); break;
		case 2: fz_paint_affine_alpha_N_lerp(dp, sp, sw, sh, u, v, fa, fb, w, 2, alpha, hp); break;
		case 4: fz_paint_affine_alpha_N_lerp(dp, sp, sw, sh, u, v, fa, fb, w, 4, alpha, hp); break;
		default: fz_paint_affine_alpha_N_lerp(dp, sp, sw, sh, u, v, fa, fb, w, n, alpha, hp); break;
		}
	}
}

static void
fz_paint_affine_g2rgb_lerp(byte *dp, byte *sp, int sw, int sh, int u, int v, int fa, int fb, int w, int n, int alpha, byte *color/*unused*/, byte *hp)
{
	if (alpha == 255)
	{
		fz_paint_affine_solid_g2rgb_lerp(dp, sp, sw, sh, u, v, fa, fb, w, hp);
	}
	else if (alpha > 0)
	{
		fz_paint_affine_alpha_g2rgb_lerp(dp, sp, sw, sh, u, v, fa, fb, w, alpha, hp);
	}
}

static void
fz_paint_affine_near(byte *dp, byte *sp, int sw, int sh, int u, int v, int fa, int fb, int w, int n, int alpha, byte *color/*unused */, byte *hp)
{
	if (alpha == 255)
	{
		switch (n)
		{
		case 1: fz_paint_affine_N_near(dp, sp, sw, sh, u, v, fa, fb, w, 1, hp); break;
		case 2: fz_paint_affine_N_near(dp, sp, sw, sh, u, v, fa, fb, w, 2, hp); break;
		case 4: fz_paint_affine_N_near(dp, sp, sw, sh, u, v, fa, fb, w, 4, hp); break;
		default: fz_paint_affine_N_near(dp, sp, sw, sh, u, v, fa, fb, w, n, hp); break;
		}
	}
	else if (alpha > 0)
	{
		switch (n)
		{
		case 1: fz_paint_affine_alpha_N_near(dp, sp, sw, sh, u, v, fa, fb, w, 1, alpha, hp); break;
		case 2: fz_paint_affine_alpha_N_near(dp, sp, sw, sh, u, v, fa, fb, w, 2, alpha, hp); break;
		case 4: fz_paint_affine_alpha_N_near(dp, sp, sw, sh, u, v, fa, fb, w, 4, alpha, hp); break;
		default: fz_paint_affine_alpha_N_near(dp, sp, sw, sh, u, v, fa, fb, w, n, alpha, hp); break;
		}
	}
}

static void
fz_paint_affine_g2rgb_near(byte *dp, byte *sp, int sw, int sh, int u, int v, int fa, int fb, int w, int n, int alpha, byte *color/*unused*/, byte *hp)
{
	if (alpha == 255)
	{
		fz_paint_affine_solid_g2rgb_near(dp, sp, sw, sh, u, v, fa, fb, w, hp);
	}
	else if (alpha > 0)
	{
		fz_paint_affine_alpha_g2rgb_near(dp, sp, sw, sh, u, v, fa, fb, w, alpha, hp);
	}
}

static void
fz_paint_affine_color_lerp(byte *dp, byte *sp, int sw, int sh, int u, int v, int fa, int fb, int w, int n, int alpha/*unused*/, byte *color, byte *hp)
{
	switch (n)
	{
	case 2: fz_paint_affine_color_N_lerp(dp, sp, sw, sh, u, v, fa, fb, w, 2, color, hp); break;
	case 4: fz_paint_affine_color_N_lerp(dp, sp, sw, sh, u, v, fa, fb, w, 4, color, hp); break;
	default: fz_paint_affine_color_N_lerp(dp, sp, sw, sh, u, v, fa, fb, w, n, color, hp); break;
	}
}

static void
fz_paint_affine_color_near(byte *dp, byte *sp, int sw, int sh, int u, int v, int fa, int fb, int w, int n, int alpha/*unused*/, byte *color, byte *hp)
{
	switch (n)
	{
	case 2: fz_paint_affine_color_N_near(dp, sp, sw, sh, u, v, fa, fb, w, 2, color, hp); break;
	case 4: fz_paint_affine_color_N_near(dp, sp, sw, sh, u, v, fa, fb, w, 4, color, hp); break;
	default: fz_paint_affine_color_N_near(dp, sp, sw, sh, u, v, fa, fb, w, n, color, hp); break;
	}
}

/* RJW: The following code was originally written to be sensitive to
 * FLT_EPSILON. Given the way the 'minimum representable difference'
 * between 2 floats changes size as we scale, we now pick a larger
 * value to ensure idempotency even with rounding problems. The
 * value we pick is still far smaller than would ever show up with
 * antialiasing.
 */
#define MY_EPSILON 0.001

void
fz_gridfit_matrix(fz_matrix *m)
{
	if (fabsf(m->b) < FLT_EPSILON && fabsf(m->c) < FLT_EPSILON)
	{
		if (m->a > 0)
		{
			float f;
			/* Adjust left hand side onto pixel boundary */
			f = (float)(int)(m->e);
			if (f - m->e > MY_EPSILON)
				f -= 1.0; /* Ensure it moves left */
			m->a += m->e - f; /* width gets wider as f <= m->e */
			m->e = f;
			/* Adjust right hand side onto pixel boundary */
			f = (float)(int)(m->a);
			if (m->a - f > MY_EPSILON)
				f += 1.0; /* Ensure it moves right */
			m->a = f;
		}
		else if (m->a < 0)
		{
			float f;
			/* Adjust right hand side onto pixel boundary */
			f = (float)(int)(m->e);
			if (m->e - f > MY_EPSILON)
				f += 1.0; /* Ensure it moves right */
			m->a += m->e - f; /* width gets wider (more -ve) */
			m->e = f;
			/* Adjust left hand side onto pixel boundary */
			f = (float)(int)(m->a);
			if (f - m->a > MY_EPSILON)
				f -= 1.0; /* Ensure it moves left */
			m->a = f;
		}
		if (m->d > 0)
		{
			float f;
			/* Adjust top onto pixel boundary */
			f = (float)(int)(m->f);
			if (f - m->f > MY_EPSILON)
				f -= 1.0; /* Ensure it moves upwards */
			m->d += m->f - f; /* width gets wider as f <= m->f */
			m->f = f;
			/* Adjust bottom onto pixel boundary */
			f = (float)(int)(m->d);
			if (m->d - f > MY_EPSILON)
				f += 1.0; /* Ensure it moves down */
			m->d = f;
		}
		else if (m->d < 0)
		{
			float f;
			/* Adjust bottom onto pixel boundary */
			f = (float)(int)(m->f);
			if (m->f - f > MY_EPSILON)
				f += 1.0; /* Ensure it moves down */
			m->d += m->f - f; /* width gets wider (more -ve) */
			m->f = f;
			/* Adjust top onto pixel boundary */
			f = (float)(int)(m->d);
			if (f - m->d > MY_EPSILON)
				f -= 1.0; /* Ensure it moves up */
			m->d = f;
		}
	}
	else if (fabsf(m->a) < FLT_EPSILON && fabsf(m->d) < FLT_EPSILON)
	{
		if (m->b > 0)
		{
			float f;
			/* Adjust left hand side onto pixel boundary */
			f = (float)(int)(m->f);
			if (f - m->f > MY_EPSILON)
				f -= 1.0; /* Ensure it moves left */
			m->b += m->f - f; /* width gets wider as f <= m->f */
			m->f = f;
			/* Adjust right hand side onto pixel boundary */
			f = (float)(int)(m->b);
			if (m->b - f > MY_EPSILON)
				f += 1.0; /* Ensure it moves right */
			m->b = f;
		}
		else if (m->b < 0)
		{
			float f;
			/* Adjust right hand side onto pixel boundary */
			f = (float)(int)(m->f);
			if (m->f - f > MY_EPSILON)
				f += 1.0; /* Ensure it moves right */
			m->b += m->f - f; /* width gets wider (more -ve) */
			m->f = f;
			/* Adjust left hand side onto pixel boundary */
			f = (float)(int)(m->b);
			if (f - m->b > MY_EPSILON)
				f -= 1.0; /* Ensure it moves left */
			m->b = f;
		}
		if (m->c > 0)
		{
			float f;
			/* Adjust top onto pixel boundary */
			f = (float)(int)(m->e);
			if (f - m->e > MY_EPSILON)
				f -= 1.0; /* Ensure it moves upwards */
			m->c += m->e - f; /* width gets wider as f <= m->e */
			m->e = f;
			/* Adjust bottom onto pixel boundary */
			f = (float)(int)(m->c);
			if (m->c - f > MY_EPSILON)
				f += 1.0; /* Ensure it moves down */
			m->c = f;
		}
		else if (m->c < 0)
		{
			float f;
			/* Adjust bottom onto pixel boundary */
			f = (float)(int)(m->e);
			if (m->e - f > MY_EPSILON)
				f += 1.0; /* Ensure it moves down */
			m->c += m->e - f; /* width gets wider (more -ve) */
			m->e = f;
			/* Adjust top onto pixel boundary */
			f = (float)(int)(m->c);
			if (f - m->c > MY_EPSILON)
				f -= 1.0; /* Ensure it moves up */
			m->c = f;
		}
	}
}

/* Draw an image with an affine transform on destination */

static void
fz_paint_image_imp(fz_pixmap *dst, fz_bbox scissor, fz_pixmap *shape, fz_pixmap *img, fz_matrix ctm, byte *color, int alpha)
{
	byte *dp, *sp, *hp;
	int u, v, fa, fb, fc, fd;
	int x, y, w, h;
	int sw, sh, n, hw;
	fz_matrix inv;
	fz_bbox bbox;
	int dolerp;
	void (*paintfn)(byte *dp, byte *sp, int sw, int sh, int u, int v, int fa, int fb, int w, int n, int alpha, byte *color, byte *hp);

	/* grid fit the image */
	fz_gridfit_matrix(&ctm);

	/* turn on interpolation for upscaled and non-rectilinear transforms */
	dolerp = 0;
	if (!fz_is_rectilinear(ctm))
		dolerp = 1;
	if (sqrtf(ctm.a * ctm.a + ctm.b * ctm.b) > img->w)
		dolerp = 1;
	if (sqrtf(ctm.c * ctm.c + ctm.d * ctm.d) > img->h)
		dolerp = 1;

	/* except when we shouldn't, at large magnifications */
	if (!img->interpolate)
	{
		if (sqrtf(ctm.a * ctm.a + ctm.b * ctm.b) > img->w * 2)
			dolerp = 0;
		if (sqrtf(ctm.c * ctm.c + ctm.d * ctm.d) > img->h * 2)
			dolerp = 0;
	}

	bbox = fz_bbox_covering_rect(fz_transform_rect(ctm, fz_unit_rect));
	bbox = fz_intersect_bbox(bbox, scissor);
	x = bbox.x0;
	if (shape && shape->x > x)
		x = shape->x;
	y = bbox.y0;
	if (shape && shape->y > y)
		y = shape->y;
	w = bbox.x1;
	if (shape && shape->x + shape->w < w)
		w = shape->x + shape->w;
	w -= x;
	h = bbox.y1;
	if (shape && shape->y + shape->h < h)
		h = shape->y + shape->h;
	h -= y;
	if (w < 0 || h < 0)
		return;

	/* map from screen space (x,y) to image space (u,v) */
	inv = fz_scale(1.0f / img->w, 1.0f / img->h);
	inv = fz_concat(inv, ctm);
	inv = fz_invert_matrix(inv);

	fa = (int)(inv.a *= 65536.0f);
	fb = (int)(inv.b *= 65536.0f);
	fc = (int)(inv.c *= 65536.0f);
	fd = (int)(inv.d *= 65536.0f);
	inv.e *= 65536.0f;
	inv.f *= 65536.0f;

	/* Calculate initial texture positions. Do a half step to start. */
	/* Bug 693021: Keep calculation in float for as long as possible to
	 * avoid overflow. */
	u = (int)((inv.a * x) + (inv.c * y) + inv.e + ((inv.a + inv.c) * .5f));
	v = (int)((inv.b * x) + (inv.d * y) + inv.f + ((inv.b + inv.d) * .5f));

	/* RJW: The following is voodoo. No idea why it works, but it gives
	 * the best match between scaled/unscaled/interpolated/non-interpolated
	 * that we have found. */
	if (dolerp) {
		u -= 32768;
		v -= 32768;
	}

	dp = dst->samples + (unsigned int)(((y - dst->y) * dst->w + (x - dst->x)) * dst->n);
	n = dst->n;
	sp = img->samples;
	sw = img->w;
	sh = img->h;
	if (shape)
	{
		hw = shape->w;
		hp = shape->samples + (unsigned int)(((y - shape->y) * hw) + x - shape->x);
	}
	else
	{
		hw = 0;
		hp = NULL;
	}

	/* TODO: if (fb == 0 && fa == 1) call fz_paint_span */

	if (dst->n == 4 && img->n == 2)
	{
		assert(!color);
		if (dolerp)
			paintfn = fz_paint_affine_g2rgb_lerp;
		else
			paintfn = fz_paint_affine_g2rgb_near;
	}
	else
	{
		if (dolerp)
		{
			if (color)
				paintfn = fz_paint_affine_color_lerp;
			else
				paintfn = fz_paint_affine_lerp;
		}
		else
		{
			if (color)
				paintfn = fz_paint_affine_color_near;
			else
				paintfn = fz_paint_affine_near;
		}
	}

	while (h--)
	{
		paintfn(dp, sp, sw, sh, u, v, fa, fb, w, n, alpha, color, hp);
		dp += dst->w * n;
		hp += hw;
		u += fc;
		v += fd;
	}
}

void
fz_paint_image_with_color(fz_pixmap *dst, fz_bbox scissor, fz_pixmap *shape, fz_pixmap *img, fz_matrix ctm, byte *color)
{
	assert(img->n == 1);
	fz_paint_image_imp(dst, scissor, shape, img, ctm, color, 255);
}

void
fz_paint_image(fz_pixmap *dst, fz_bbox scissor, fz_pixmap *shape, fz_pixmap *img, fz_matrix ctm, int alpha)
{
	assert(dst->n == img->n || (dst->n == 4 && img->n == 2));
	fz_paint_image_imp(dst, scissor, shape, img, ctm, NULL, alpha);
}
