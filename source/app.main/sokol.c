#include <sokol/sokol_app.h>
#include <sokol/sokol_gfx.h>

#ifdef _WIN32
#include <Windows.h>
#endif

static struct { sg_pass_action pass_action; } state;

sg_pass_action* get_pass_action(void) { return &state.pass_action; }

void init(void) {
  sg_setup(&(sg_desc){.gl_force_gles2 = sapp_gles2(),
                      .mtl_device = sapp_metal_get_device(),
                      .mtl_renderpass_descriptor_cb = sapp_metal_get_renderpass_descriptor,
                      .mtl_drawable_cb = sapp_metal_get_drawable,
                      .d3d11_device = sapp_d3d11_get_device(),
                      .d3d11_device_context = sapp_d3d11_get_device_context(),
                      .d3d11_render_target_view_cb = sapp_d3d11_get_render_target_view,
                      .d3d11_depth_stencil_view_cb = sapp_d3d11_get_depth_stencil_view});
  state.pass_action = (sg_pass_action){.colors[0] = {.action = SG_ACTION_CLEAR, .val = {1.0f, 0.0f, 0.0f, 1.0f}}};
  //__dbgui_setup(1);

  
# ifdef _WIN32
  const HWND handle = (HWND)sapp_win32_get_hwnd();
  const HICON icon = LoadIconA(GetModuleHandleW(NULL), "MAINICON");

  SendMessage(handle, WM_SETICON, ICON_SMALL, (LPARAM)icon);
  SendMessage(handle, WM_SETICON, ICON_BIG, (LPARAM)icon);
# endif
}

void cleanup(void) {
  //__dbgui_shutdown();
  sg_shutdown();
}

sapp_desc sokol_main2(int argc, char* argv[], void (*frame_cb)(void)) {
  return (sapp_desc){
      .init_cb = init,
      .frame_cb = frame_cb,
      .cleanup_cb = cleanup,
      .width = 400,
      .height = 300,
      .gl_force_gles2 = true,
      .window_title = "Clear (sokol app)",
  };
}
