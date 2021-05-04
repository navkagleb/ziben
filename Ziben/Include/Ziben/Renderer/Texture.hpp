#pragma once

#include "GraphicsCore.hpp"
#include "Ziben/Utility/Reference.hpp"

namespace Ziben {

    class Texture {
    public:
//        static Ref<Texture> Create(const std::string& filepath);
//        static Ref<Texture> Create(uint32_t width, uint32_t height);

        static void Bind(const Ref<Texture>& texture, uint32_t slot);

    public:
        Texture() = default;
        virtual ~Texture() = default;

        [[nodiscard]] virtual uint32_t GetWidth() const = 0;
        [[nodiscard]] virtual uint32_t GetHeight() const = 0;

        virtual void SetData(void* data, uint32_t size) = 0;

    }; // class Texture

    class Texture2D : public Texture {
    public:
        static Ref<Texture2D> Create(const std::string& filepath);
        static Ref<Texture2D> Create(uint32_t width, uint32_t height);

        static void Bind(const Ref<Texture2D>& texture2D, uint32_t slot = 0);
        static void Unbind();

    public:
        explicit Texture2D(const std::string& filepath);
        Texture2D(uint32_t width, uint32_t height);
        ~Texture2D() override;

        [[nodiscard]] uint32_t GetHandle() const { return m_Handle; }
        [[nodiscard]] uint32_t GetWidth() const override { return m_Width; }
        [[nodiscard]] uint32_t GetHeight() const override { return m_Height; }

        void SetData(void* data, uint32_t size) override;

    public:
        bool operator !=(const Texture2D& other) const;
        bool operator ==(const Texture2D& other) const;

    private:
        HandleType m_Handle;
        uint32_t   m_Width;
        uint32_t   m_Height;
        uint32_t   m_InternalFormat;
        uint32_t   m_DataFormat;

    }; // class Texture2D

} // namespace Ziben