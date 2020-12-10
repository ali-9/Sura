﻿
#pragma once
#pragma warning(disable : 26812)
#include "VkBootstrap.h"
#include "vk_deletionQueue.h"
#include "vk_initializers.h"
#include "vk_renderable.h"
#include <unordered_map>
#include <vk_mesh.h>
#include <vk_types.h>

class VulkanEngine {
public:
  struct SDL_Window *m_window{nullptr};

  // initializes everything in the engine
  void init();

  // shuts down the engine
  void cleanup();

  // draw loop
  void draw();

  // run main loop
  void run();

private:
  void init_vulkan();
  void init_allocator();
  void init_depth_buffer();
  void init_swapchain();
  void init_commands();
  void init_render_pass();
  void init_framebuffers();
  void init_sync_structs();
  bool load_shader_module(const char *file_path, VkShaderModule *out);
  void init_pipelines();

  // meshes
  void load_meshes();
  void upload_mesh(Mesh &mesh);
  void init_triangle_mesh_pipeline(vkinit::PipelineBuilder &builder);
  void init_scene();
  void draw_objects(VkCommandBuffer cmd, RenderObject *first, size_t count);

private:
  bool m_isInitialized{false};
  int m_frameNumber{0};

  VkExtent2D m_windowExtent{1700, 900};
  VkInstance m_instance;
  VkDebugUtilsMessengerEXT m_debug_messenger;
  VkPhysicalDevice m_physical_device;
  VkDevice m_device;
  VkSurfaceKHR m_surface;

  // swapchain
  VkSwapchainKHR m_swapchain;
  VkFormat m_swapchain_image_format;
  std::vector<VkImage> m_swapchain_images;
  std::vector<VkImageView> m_swapchain_image_views;

  // depth
  VkFormat m_depth_format;
  VkImageView m_depth_image_view;
  AllocatedImage m_depth_image;

  // commands
  VkQueue m_graphics_queue;
  uint32_t m_graphics_queue_family;
  VkCommandPool m_command_pool;
  VkCommandBuffer m_main_command_buffer;

  // renderpass
  VkRenderPass m_render_pass;
  std::vector<VkFramebuffer> m_framebuffers;

  // sync
  VkSemaphore m_present_semaphore, m_render_semaphore;
  VkFence m_render_fence;

  // pipelines

  VkPipelineLayout m_triangle_pipeline_layout;
  VkPipeline m_triangle_pipeline;

  VkPipeline m_mesh_pipeline;
  VkPipelineLayout m_mesh_pipeline_layout;

  // amd allocator
  VmaAllocator m_allocator;

  // meshes
  Mesh m_triangle_mesh;
  Mesh m_monkey;

  // deletion queue of vulkan objects
  DeletionQueue m_main_deletion_queue;

  // renderables
  std::vector<RenderObject> m_renderables;
  std::unordered_map<std::string, Material> m_materials;
  std::unordered_map<std::string, Mesh> m_meshes;

  Material *create_material(VkPipeline pipeline, VkPipelineLayout layout,
                            const std::string &name);
  Material *get_material(const std::string &name);
  Mesh *get_mesh(const std::string &name);
  void draw_objects(VkCommandBuffer cmd, RenderObject *first, int count);
};
