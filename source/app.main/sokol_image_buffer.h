#pragma once

#include <EAStdC/EASprintf.h>
#include <imgui/imgui.h>
#include <ros/cast.h>
#include <sokol/sokol_gfx.h>
#include <stb/stb_image.h>

#include <cstdio>
#include <memory>

union color {
  unsigned int integer;
  unsigned char rgba[4];

  operator unsigned int() const { return integer; }

  color(int i) : integer($<unsigned int>(i)) {}
  color(unsigned int ui) : integer(ui) {}

  void operator=(unsigned int ui) { integer = ui; }
  void operator=(int i) { integer = $<unsigned int>(i); }
};

static_assert(sizeof(color) == 4);

struct sokol_image_buffer {
  sg_image image;
  color *pixelbuffer;
  int width;
  int height;

  size_t byteSize() const { return sizeof(color) * width * height; }
  ImTextureID imguiTexture() const { return (ImTextureID)(uintptr_t)image.id; }

  void setupTexture() {
    sg_image_desc img_desc;
    memset(&img_desc, 0, sizeof(img_desc));
    img_desc.width = width;
    img_desc.height = height;
    img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
    img_desc.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
    img_desc.wrap_v = SG_WRAP_CLAMP_TO_EDGE;
    img_desc.min_filter = SG_FILTER_LINEAR;
    img_desc.mag_filter = SG_FILTER_LINEAR;
    img_desc.usage = SG_USAGE_DYNAMIC;
    img_desc.label = "loaded image";
    image = sg_make_image(&img_desc);
  }

  sokol_image_buffer() = delete;
  sokol_image_buffer(const sokol_image_buffer &) = delete;
  sokol_image_buffer(const sokol_image_buffer &&) = delete;

  sokol_image_buffer(int width_, int height_, color *pixelbuffer_)
      : image({SG_INVALID_ID}), width(width_), height(height_), pixelbuffer(pixelbuffer_) {
    if (pixelbuffer == nullptr) {
      pixelbuffer = reinterpret_cast<color *>(std::malloc(byteSize()));
    }
    setupTexture();
    EA::StdC::Printf("Create new image %d (%d, %d) \n", image.id, width, height);
  }

  sokol_image_buffer(int width_, int height_) : sokol_image_buffer(width_, height_, nullptr) {}

  sokol_image_buffer(const char *path) : image({SG_INVALID_ID}), width(0), height(0), pixelbuffer(nullptr) {
    int n;
    pixelbuffer = reinterpret_cast<color *>(stbi_load(path, &width, &height, &n, 4));

    if (pixelbuffer == nullptr) {
      EA::StdC::Fprintf(stderr, "failed to open image %s: %s \n", path, stbi_failure_reason());
      return;
    }

    setupTexture();
    EA::StdC::Printf("Loaded image %d (%d, %d) %s \n", image.id, width, height, path);
  }

  ~sokol_image_buffer() {
    EA::StdC::Printf("Destroyed image %d (%d, %d) \n", image.id, width, height);
    if (pixelbuffer) {
      std::free(pixelbuffer);
      pixelbuffer = nullptr;
    }
    if (image.id != SG_INVALID_ID) {
      sg_destroy_image(image);
      image.id = SG_INVALID_ID;
    }
    width = 0;
    height = 0;
  }

  bool hasFailed() const { return pixelbuffer == nullptr; }

  void uploadTexture() {
    sg_image_content data;
    std::memset(&data, 0, sizeof(sg_image_content));
    data.subimage[0][0].ptr = pixelbuffer;
    data.subimage[0][0].size = sizeof(uint32_t) * width * height;

    sg_update_image(image, &data);
  }

  void clear() { memset(pixelbuffer, 0, byteSize()); }

  bool putpixel(int x, int y, color c) {
    if (x < 0) return false;
    if (y < 0) return false;
    if (x >= width) return false;
    if (y >= height) return false;

    pixelbuffer[x + width * y] = c;
    return true;
  }

  color getpixel(int x, int y) const {
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= width) x = width - 1;
    if (y >= height) y = height - 1;

    return pixelbuffer[x + width * y];
  }

  void copy(const sokol_image_buffer &src) {
    assert(src.width == width);
    assert(src.height == height);

    std::memcpy(pixelbuffer, src.pixelbuffer, byteSize());
  }
};
