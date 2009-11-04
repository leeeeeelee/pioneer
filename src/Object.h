#ifndef _OBJECT_H
#define _OBJECT_H

class Object {
	public:
	enum Type { OBJECT, BODY, MODELBODY, DYNAMICBODY, SHIP, PLAYER, SPACESTATION, PLANET, STAR, CARGOBODY, CITYONPLANET, PROJECTILE, MISSILE, HYPERSPACECLOUD };
	virtual Type GetType() const { return OBJECT; }
	virtual bool IsType(Type c) const { return GetType() == c; }
};
#define OBJDEF(__thisClass,__parentClass,__TYPE) \
	virtual Object::Type GetType() const { return Object::__TYPE; } \
	virtual bool IsType(Type c) const { \
	if (__thisClass::GetType() == (c)) return true; \
	else return __parentClass::IsType(c); }
#endif /* _OBJECT_H */
