// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{

    /// @todo docs & test
    struct hexdump
    {
        hexdump(const sak::const_storage& storage)
            : m_storage(storage),
              m_max_size(storage.m_size)
        {
            assert(m_storage.m_data);
            assert(m_storage.m_size);
            assert(m_max_size);
        }

        void set_max_size(uint32_t max_size)
        {
            assert(max_size > 0);
            m_max_size = max_size;
        }

        sak::const_storage m_storage;

        uint32_t m_max_size;
    };

    inline std::ostream& operator<<(std::ostream& out, const hexdump& hex)
    {
        const uint8_t* data = hex.m_storage.m_data;
        uint32_t size = std::min(hex.m_storage.m_size, hex.m_max_size);

        assert(data);
        assert(size > 0);

        // don't change formatting for out
        std::ostream s(out.rdbuf());
        s << std::hex << std::setfill('0');

        std::string buf;
        buf.reserve(17); // premature optimization

        for (uint32_t i = 0; i < size; ++i)
        {
            if ((i % 16) == 0)
            {
                if (i)
                {
                    s << "  " << buf << std::endl;
                    buf.clear();
                }
                s << "  " << std::setw(4) << i << ' ';
            }

            uint8_t c = data[i];

            s << ' ' << std::setw(2) << (uint32_t) c;
            buf += (0x20 <= c && c <= 0x7e) ? c : '.';
        }

        if (size % 16)
        {
            s << std::string(3*(size % 16 ), ' ');
        }

        s << "  " << buf << std::endl;

        if (size < hex.m_storage.m_size)
        {
            uint32_t last_row = (hex.m_storage.m_size / 16) * 16;

            s << "  " << "...." << std::endl;
            s << "  " << std::setw(4) << last_row  << ' ' << std::endl;
        }

        return out;
    }
}
