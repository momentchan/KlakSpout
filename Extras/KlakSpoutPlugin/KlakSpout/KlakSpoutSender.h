#pragma once

#include "KlakSpoutGlobals.h"

namespace klakspout
{
    // Spout sender object
    class Sender
    {
    public:

        // Identifier for the receier list
        int id_;

        // Spout sender infromation
        string name_;
        unsigned int width_, height_;
        DXGI_FORMAT format_;

        // D3D resources and handlers
        ID3D11Texture2D * texture_;
        ID3D11ShaderResourceView* view_;
        HANDLE handle_;

        // Constructor (only for basic initialization)
        Sender(int id, string name, unsigned int width, unsigned int height)
            : id_(id), name_(name),
              width_(width), height_(height), format_(DXGI_FORMAT_R8G8B8A8_UNORM),
              texture_(nullptr), view_(nullptr), handle_(nullptr)
        {
        }

        // Destructor
        ~Sender()
        {
            Cleanup();
        }

        // Has the texture been already up?
        bool IsReady()
        {
            return texture_;
        }

        // Releases all the resources. Can be called multiple times.
        void Cleanup()
        {
            if (view_)
            {
                view_->Release();
                view_ = nullptr;
            }

            if (texture_)
            {
                Globals::get().sender_names_->ReleaseSenderName(name_.c_str());
                texture_->Release();
                texture_ = nullptr;
            }
        }

        // Sets up the resources and start using them.
        void Setup()
        {
            // Do nothing if ready.
            if (IsReady()) return;

            auto & g = Globals::get();

            // Create a shared texture.
            auto res_spout = g.spout_->CreateSharedDX11Texture(g.d3d11_, width_, height_, format_, &texture_, handle_);

            if (!res_spout)
            {
                DEBUG_LOG("CreateSharedDX11Texture failed.");
                return;
            }

            // Create a resource view for the shared texture.
            auto res_d3d = g.d3d11_->CreateShaderResourceView(texture_, nullptr, &view_);

            if (FAILED(res_d3d))
            {
                DEBUG_LOG("CreateShaderResourceView failed (%x).", res_d3d);
                Cleanup();
                return;
            }

            // Start sharing it with creating a Spout sender.
            res_spout = g.sender_names_->CreateSender(name_.c_str(), width_, height_, handle_, format_);

            if (!res_spout)
            {
                DEBUG_LOG("CreateSender failed.");
                Cleanup();
                return;
            }

            DEBUG_LOG("Sender created.");
        }
    };
}