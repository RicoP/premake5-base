#define SOKOL_GLCORE33
#define SOKOL_IMGUI_IMPL

#include <ImGuiFileBrowser/FileBrowser/ImGuiFileBrowser.h>
#include <imgui/imgui.h>
#include <ros/range.h>
#include <ros/refptr.h>
#include <sokol/sokol_app.h>
#include <sokol/sokol_gfx.h>
#include <sokol/sokol_time.h>
#include <sokol/util/sokol_imgui.h>

#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  define NOMINMAX
#  include <Windows.h>

void setWindowIcon() {
  const HWND handle = (HWND)sapp_win32_get_hwnd();
  const HICON icon = LoadIconA(GetModuleHandleW(NULL), "MAINICON");

  SendMessage(handle, WM_SETICON, ICON_SMALL, (LPARAM)icon);
  SendMessage(handle, WM_SETICON, ICON_BIG, (LPARAM)icon);
}
#else
void setWindowIcon() {}
#endif

static uint64_t last_time = 0;
static bool show_test_window = false;
static bool show_metrics_window = false;

static sg_pass_action pass_action;

imgui_addons::ImGuiFileBrowser file_dialog;  // As a class member or globally

// Now inside any function
void showMainMenu() {
  bool open = false;
  bool save = false;
  bool about = false;
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open")) open = true;
      if (ImGui::MenuItem("Save")) save = true;

      ImGui::Separator();

      if (ImGui::MenuItem("Exit")) {
        sapp_request_quit();
      }

      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("View")) {
      if (ImGui::MenuItem("Demo window", nullptr, show_test_window)) {
        show_test_window = !show_test_window;
      }
      if (ImGui::MenuItem("Imgui Metrics window", nullptr, show_metrics_window)) {
        show_metrics_window = !show_metrics_window;
      }
      if (ImGui::MenuItem("About")) {
        about = true;
      }

      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  // Remember the name to ImGui::OpenPopup() and showFileDialog() must be same...
  if (open) ImGui::OpenPopup("Open File");
  if (save) ImGui::OpenPopup("Save File");
  if (about) ImGui::OpenPopup("about");

  /* Optional third parameter. Support opening only compressed rar/zip files.
   * Opening any other file will show error, return false and won't close the dialog.
   */
  if (file_dialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310),
                                 ".png,.jpg,.tga,.bmp,.psd,.gif,.hdr,.pic,.pnm")) {
  }
  if (file_dialog.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310),
                                 ".png,.jpg,.tga,.bmp,.psd,.gif,.hdr,.pic,.pnm")) {
  }

  if (ImGui::BeginPopupModal("about", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("(c) Rico P");

    if (ImGui::Button("OK")) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void init(void) {
  setWindowIcon();

  // setup sokol-gfx, sokol-time and sokol-imgui
  sg_desc desc = {};
  desc.mtl_device = sapp_metal_get_device();
  desc.mtl_renderpass_descriptor_cb = sapp_metal_get_renderpass_descriptor;
  desc.mtl_drawable_cb = sapp_metal_get_drawable;
  desc.d3d11_device = sapp_d3d11_get_device();
  desc.d3d11_device_context = sapp_d3d11_get_device_context();
  desc.d3d11_render_target_view_cb = sapp_d3d11_get_render_target_view;
  desc.d3d11_depth_stencil_view_cb = sapp_d3d11_get_depth_stencil_view;
  desc.gl_force_gles2 = sapp_gles2();
  sg_setup(&desc);
  stm_setup();

  // use sokol-imgui with all default-options (we're not doing
  // multi-sampled rendering or using non-default pixel formats)
  simgui_desc_t simgui_desc = {};
  simgui_setup(&simgui_desc);

  // initial clear color
  pass_action.colors[0].action = SG_ACTION_CLEAR;
  pass_action.colors[0].val[0] = 0.0f;
  pass_action.colors[0].val[1] = 0.5f;
  pass_action.colors[0].val[2] = 0.7f;
  pass_action.colors[0].val[3] = 1.0f;
}

void frame(void) {
  const int width = sapp_width();
  const int height = sapp_height();
  const double delta_time = stm_sec(stm_laptime(&last_time));
  simgui_new_frame(width, height, delta_time);

  showMainMenu();

  if (show_metrics_window) {
    ImGui::ShowMetricsWindow(&show_metrics_window);
  }
  if (show_test_window) {
    ImGui::ShowDemoWindow(&show_test_window);
  }

  // the sokol_gfx draw pass
  sg_begin_default_pass(&pass_action, width, height);
  simgui_render();
  sg_end_pass();
  sg_commit();
}

void cleanup(void) {
  simgui_shutdown();
  sg_shutdown();
}

void input(const sapp_event *event) { simgui_handle_event(event); }

sapp_desc sokol_imgui_desc() {
  sapp_desc desc = {};
  desc.init_cb = init;
  desc.frame_cb = frame;
  desc.cleanup_cb = cleanup;
  desc.event_cb = input;
  desc.width = 1280;
  desc.height = 720;
  desc.gl_force_gles2 = true;
  desc.window_title = "premake5-test";
  desc.ios_keyboard_resizes_canvas = false;
  return desc;
}

extern "C" int appmain(int, char *[]) {
  sapp_desc desc = sokol_imgui_desc();
  return sapp_run(&desc);
}
