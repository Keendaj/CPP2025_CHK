#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include <any>
#include <vector>
#include <utility>
#include <stdexcept>
#include <iostream>

template<class Class, class Function>
class wrapper;

class Engine
{
private:
    class IWrapper
    {
    public:
        virtual ~IWrapper() = default;
        virtual std::any execute(const std::vector<std::pair<std::string, std::any>>& overrides) = 0;
        virtual const std::type_info& getReturnType() const = 0;
    };

    template<class Class, class Function>
    class WrapperImpl : public IWrapper
    {
    private:
        wrapper<Class, Function> wrapped;
        
    public:
        WrapperImpl(const wrapper<Class, Function>& wrapped)
            : wrapped(wrapped)
        {
        }

        std::any execute(const std::vector<std::pair<std::string, std::any>>& overrides) override
        {
            using ReturnType = typename wrapper<Class, Function>::ReturnType;
    
            if constexpr (std::is_void_v<ReturnType>) {
                wrapped.execute(overrides);
                return std::any{};
            } else {
                return wrapped.execute(overrides);
            }
        }

        std::type_info const& getReturnType() const override
        {
            return wrapper<Class, Function>::getReturnTypeInfo();
        }
    };

    std::unordered_map<std::string, std::unique_ptr<IWrapper>> mutable s;

public:
    template<class Class, class Function>
    void registerCommand(const std::string& Name,
        const wrapper<Class, Function>& wrapped) const
    {
        if (s.find(Name) != s.end())
        {
            throw std::runtime_error(" '" + Name + "' already registered");
        }

        s[Name] = std::make_unique<WrapperImpl<Class, Function>>(wrapped);
    }

    std::any execute(const std::string& Name,
                    const std::vector<std::pair<std::string, std::any>>& overrides = {})
    {
        auto it = s.find(Name);
        if (it == s.end()) {
            throw std::runtime_error(" '" + Name + "' not found");
        }
        
        if (it->second->getReturnType() == typeid(void)) {
            it->second->execute(overrides);
            return std::any();
        }

        return it->second->execute(overrides);
    }

    const std::type_info& getReturnType(const std::string& Name) const
    {
        auto it = s.find(Name);
        if (it == s.end()) {
            throw std::runtime_error(" '" + Name + "' not found");
        }
        return it->second->getReturnType();
    }
    
    bool unregister(const std::string& Name)
    {
        return s.erase(Name) > 0;
    }

    void clear()
    {
        s.clear();
    }
};