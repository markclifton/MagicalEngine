#pragma once

struct RenderComponent {
};

struct BatchRenderComponent {
    BatchRenderComponent() {}
    BatchRenderComponent(int id) : batch_id(id) {}
    int batch_id;
};

struct IndexedRenderComponent {
};