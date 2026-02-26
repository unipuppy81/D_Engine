#pragma once

#include <string>
#include <memory>
#include <vector>
#include <atomic>

enum class ObjectType : uint8_t{
	None,
	GameObject,
	Component,
	Resource,			// Mesh, Texture, Material
	Scene,
};



class Object : public std::enable_shared_from_this<Object>{
protected:
	Object(ObjectType type);
	virtual ~Object() = default;

	// 복사 방지
	Object(const Object&) = delete;
	Object& operator=(const Object&) = delete;

	virtual void Init() {}

	// Getter
	uint64_t GetId() const { return _id; }
	ObjectType GetType() const { return _type; }
	const std::wstring& GetName() const { return _name; }
	bool IsDead() const { return _isDead; }

	// Setter
	void SetName(const std::wstring& name) { _name = name; }
	void Destroy() { _isDead = true; }

	
	template<typename T>
	std::shared_ptr<T> GetShared() { return std::static_pointer_cast<T>(shared_from_this()); }

protected:
	ObjectType _type = ObjectType::None;
	std::wstring _name;
	uint64_t _id = 0;
	bool _isDead = false;

private:
	static std::atomic<uint64_t> s_idCounter;
};