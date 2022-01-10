#pragma once

#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fontconfig/fontconfig.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <xcb/render.h>
#include <xcb/xcb.h>
#include <xcb/xcb_renderutil.h>
#include <xcb/xcb_xrm.h>

typedef struct {
  FcChar32 *str;
  unsigned int length;
} utf_holder;

typedef struct {
  FcPattern **patterns;
  uint8_t length;
} xcbft_patterns_holder;

typedef struct {
  FT_Face *faces;
  uint8_t length;
  FT_Library library;
} xcbft_face_holder;

typedef struct {
  xcb_render_glyphset_t glyphset;
  FT_Vector advance;
} xcbft_glyphset_and_advance;

// signatures
bool xcbft_init(void);
void xcbft_done(void);
FcPattern *xcbft_query_fontsearch(FcChar8 *);
xcbft_face_holder xcbft_query_by_char_support(FcChar32, const FcPattern *, long);
xcbft_patterns_holder xcbft_query_fontsearch_all(FcStrSet *);
double xcbft_get_pixel_size(xcbft_patterns_holder patterns);
xcbft_face_holder xcbft_load_faces(xcbft_patterns_holder, long);
FcStrSet *xcbft_extract_fontsearch_list(const char *);
void xcbft_patterns_holder_destroy(xcbft_patterns_holder);
void xcbft_face_holder_destroy(xcbft_face_holder);
FT_Vector xcbft_draw_text(xcb_connection_t *, xcb_drawable_t, int16_t, int16_t, utf_holder, xcb_render_color_t,
                          xcbft_face_holder, long);
xcb_render_picture_t xcbft_create_pen(xcb_connection_t *, xcb_render_color_t);
xcbft_glyphset_and_advance xcbft_load_glyphset(xcb_connection_t *, xcbft_face_holder, utf_holder, long);
FT_Vector xcbft_load_glyph(xcb_connection_t *, xcb_render_glyphset_t, FT_Face, int);
long xcbft_get_dpi(xcb_connection_t *);
xcb_pixmap_t xcbft_create_text_pixmap(xcb_connection_t *, utf_holder, xcb_render_color_t, xcb_render_color_t,
                                      xcbft_patterns_holder, long);
utf_holder char_to_uint32(const char *str);
void utf_holder_destroy(utf_holder holder);
