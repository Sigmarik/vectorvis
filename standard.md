В обсуждении приняли участие:
* Абдулин Тимур       (abdullin.ta@phystech.edu)
* Абишев Тимофей      (abishev.ta@phystech.edu)
* Зотов Владислав     (hramzik.ru@yandex.ru)
* Коломийцев Даниил   (kolomiitsev.dv@phystech.edu)
* Кудряшов Илья       (kudriashov.it@phystech.edu)
* Мишенков Даниил     (mishenkov.dn@phystech.edu)
* Платонов Тимофей    (platonov.ta@phystech.edu)
* Пеньков Даниил      (penkov.da@phystech.edu)
* Ракитин Алексей     (rakitin.ad@phystech.edu)
* Солодовников Иван   (solodovnikov.ia@phystech.edu)
* Дединский Илья      (ded@ded32.ru)

Основная тема обсуждения: изменение стандарта для внедрения возможности подключения плагинов, стандартизация API.

Были выдвинуты следующие идеи:

1. Для реализации плагина необходимо сделать функции отрисовки независящими от графической библиотеки.

2. Из-за многих индивидуальных особенностей обработки canvas было предложено реализовать его в виде отдельного класса (не производного от widget) и реализовать виджет canvasView для обработки входящих событий canvas-а;
	* обсуждение ввиду актуальности других проблем отложено.

3. Было предложено добавить в параметры функций обработки событий структуру, содержащую информацию о результатах обработки предыдущих виджетов (этим же событием) вместо использования буелвского возвращаемого значения :
	+: возможность более удобного управления деревом виджетов при обработке событий,  а также обработки исключительных событий (перекрывание виджета)
	Итог: принята структура EHC(event handling context)
        struct EHC{
            Transform_stack &stack()
            bool stopped    - true if: необходимо прекратить дальнейшее прокидывание ивента
            bool overlapped - true if: перекрывание (когда ивент попал на нас, то последующие виджеты частично находятся под ним)
        }

4. Для обработки случая уничтожения виджета было предложено добавить в виджет метод bool isReadyToDelete (void), который обрабатывает ситуацию, когда родитель уничтожается и должен прежде узнать, готовы ли уничтожится его дети (пример - случай с закрытием окна при сохраненном или не сохраненном канвасе)
       в связи с этим была также предложена функция получения статуса виджета (уничтожен, готов уничтожиться, не готов уничтожится) 
       *Обсуждение отложено.
5. Предложение: реализовать обобщенную функцию обработки event-ов и передавать в нее некоторую структуру (помимо EHC) Event, которая хранит необходимые для обработки параметры;
    Предложение реализации класса Event: класс Event — базовый и хранит тип event-а, производные от него передаются в конкретную функцию-event и имеют необходимые для него параметры;
    	+: независимость от графической системы, обобщение исполняемых событий
	Предложение принято большинством (6/10). 
	В стандарт внесены следующие функции и классы:
 ```
class Event {
     	const size_t m_type;
  	public:
     	Event (size_t type) : m_type (type) {};                                            
     	size_t getType {return m_type;};
 	};

enum EventType {
  Tick 		= 0,
  MouseMove 	= 1,
  MousePressed 	= 2,
  MouseRealesed  	= 3,
  KeyboardPressed 	= 4,
  KeyboardReleased = 5
};

struct MouseMoveEvent : public Event {
  bool shift, ctrl, alt;
  Vec2d pos;
}
struct MousePressedEvent : public Event {
    MouseButton button_id;
  bool shift, ctrl, alt;
  Vec2d pos;
}
struct MouseRealesedEvent : public Event {
  MouseButton button_id;
  bool shift, ctrl, alt;
  Vec2d pos;
}
struct KeyboardPressedEvent : public Event {
  KeyCode key_id;
  bool shift, ctrl, alt;
  Vec2d pos;
}
struct KeyboardReleasedEvent : public Event {
  KeyCode key_id;
  bool shift, ctrl, alt;
  Vec2d pos;
}
struct TickEvent : public Event {
  double delta_time;
}
```

Замечание: bool shift, ctrk, alt были добавлены в клавишные и мышечные event-ы, для того чтобы проще обрабатывать случай зажатой клавиши (не хранить их непосредственно в виджетах, к примеру)}

Следующие члены-функции базового класса widget были изменены/добавлены:

```
class Widget
{
public:
  + void onEvent (const Event &event, EHC &ehc); 
protected:
  + void onTick (const TickEvent &event, EHC &ehc);
  + void onMouseMove (const MouseMoveEvent &event, EHC &ehc);
  + void onMousePressed (const MousePressedEvent &event, EHC &ehc);
             + void onMouseReleased (const MouseReleasedEvent &event, EHC &ehc);
  + void onKeyboardPressed (const KeyboardPressedEvent &event, EHC &ehc);
  + void onKeyboardReleased (const KeyboardReleasedEvent &event, EHC &ehc);
}	
```

Замечание: метод виджета „on_parent_update“ решено не обрабатывать как event, так как он, в отличие от них, может изменять значения своих параметров при прокидывании.

1. Для независти плагинов от конкретного приложения были приняты как стандарт следующие структуры и функции:
	6.1.Вектор:

```
struct Vec2d {
  double x,y;
  double length (); - длина вектора
  double length2 (); - длина вектора в квадрате
}

double dot (Vec &, Vec &)    - скалярное произведение
double cross (Vec &, Vec &) - ориентированная площадь, порожденная векторами          
Vector normalize (const Vec &v) — нормализация вектора

Следующие операторы для работы с вектором могут быть реализованы также внутри класса:
 	Vec2d operator + (const Vec2d &left, const Vec2d &right); 
 	Vec2d operator - (const Vec2d &left, const Vec2d &right);
Vec2d operator * (const Vec2d &vec, const double val);     
Vec2d operator * (const double val, const Vec2d &vec); 
Vec2d operator - (const Vec2d &vec);
Vec2d operator / (const Vec2d &left, const Vec2d &right);  
Vec2d &operator /= (Vec2d &left, const double float); 
Vec2d &operator += (Vec2d &left, const Vec2d &right);
Vec2d &operator -= (Vec2d &left, const Vec2d &right);     
Vec2d &operator *= (Vec2d &vec, const double val); 
Vec2d &operator *= (Vec2d &left, const Vec2d &right); - покоординатное умножение
Vec2d &operator /= (Vec2d &left, const Vec2d &right); - покоординатное деление
```

	6.2.Transform-ы - необходимо стандартизировать трансформы, так как виджет хранит в себе их непосредственно (иной вариант - хранить Transform * и выделять каждый раз память для получения большей свободы в их реализации, но от него решено отказаться)
```
class Transform {
  Vec2d offset;
  Vec2d scale;
public:
  Vec2d  getOffset () const;
  Vec2d setOffset (const Vec2d &new_offset);
  Vec2d  getScale () const;
  Vec2d setScale  (const Vec2d &new_scale);
  Vec2d apply (const Vec2d &vector);  - сдвигает по scale-у и прибавляет offset
  Vec2d restore (const Vec2d &vector); -  противоположность методу apply
  Transform combine (const Transform &parent_transform);
}

6.3.TransformStack — необходим стандарт для вызова методов стэка из плагина 		                               (стандартизации полей не нужна, так как стэк перердается по 					ссылке)
class TransformStack {
  void enter (const Transform &transform);
  void leave ();
  Transform top () const;
  Vec2d apply (const Vec2d &vector) const; 
  Vec2d restore (const Vec2d &vector) const;
}
```

1. Так как среди участников обсуждения есть пользователи разных графических систем, то необходимо написать обертки над системозависимыми обьектами, в чаcтности, функциями рисовки и классами рисуемых объектов.

2. Структура плагинов:
	8.1. дефолтный плагин — аналог I_Unknown:

```
class Plugin {
Plugin *tryGetInterface (size_t) = 0; - аналог  QueryInterface
void addReference (Plugin *) = 0; 
void release (Plugin *) = 0 ;
const PluginData *getPlaginData () const = 0;
}
8.2. Класс содержащий информацию о плагине:
class PluginData {
const char *getName () const;
const char *getTexturePath () const;
}
Класс, используемый для определения плагина в tryGetInterface:
enum class PluginGuid () {
Plugin = 0,
Tool = 1,
Filter = 2
}
```

Замечание:правила для tryGetInterface остаются такими же как в COM для QueryInterface.

	8.3. Плагин фильтра:

```
class Filter : public Plugin {
void applyFilter (Canvas &canvas) const;
Widget *getWidget (void);
}
```
Замечание: Filter_mask переносим из Filter в canvas как активное выделение (Selection_mask)
Замечание: позиция, передаваемая в параметры методов — в пикселях относительно левого угла текстуры.

9.
	Плагин Tool принял следующий вид:

 ```
Class Tool : public Plugin {
void setColorPalette (ColorPalette &) = 0;
void setActiveCanvas (Canvas &) = 0; 	- храним канвас в самом туле чтобы не передавать его все время в качестве параметра и для того чтобы реагировать на изменение активного канваса.
void onMainButton (ControlState &state, const Vec2d &) = 0;
void onModifier1 (const ContolState &) = 0; 
void onModifier2 (const ContolState &) = 0; 
void onModifier3 (const ContolState &) = 0; 
void onMove (const Vec2d &);
void onConfirm ();
void on_cancel ();
}
```

Замечание: Так как идеологически не логично хранить указатель на Tool_palette в canvas было принято решение хранить указатель на палитру в Tool-е, посредством его задания при добавлении в палитру.
	Следующие классы используются в Tool и имеют вид:

```
struct ControlState {
State state;
}


enum State {
Pressed, 
Released	
     }

class ColorPalette {
Color getFGColor () const; 
void setFGColor (Color);
Color getBGColor () const; 
void setBGColor (Color);
}

class Color {
   union {
uint8_t red;
uint8_t r;
   }	
   union {
uint8_t blue;
uint8_t b;
   }	
          union {
uint8_t green;
uint8_t g;
   }	
   union {
uint8_t alpha;
uint8_t a;
   }	
}
```
1.  Итоги обсуждения п.2 (Из-за многих индивидуальных особенностей обработки canvas было предложено реализовать его в виде отдельного класса (не производного от widget) и реализовать виджет canvasView для обработки входящих событий canvas-а;):

```
class Canvas {
RenderTexture &getRenderTexture ();
SelectionMask &getSelectionMask ();
void draw (const VertexArray &);
void draw (const VertexArray &, const Texture &);
unsigned int getNativeHandle (void) const;
Vec2d getSize (const Vec2d &);
void setSize (const Vec2d &);
SelectionMask &getSelectionMask ();
Color getPixel (size_t x, size_t y);
void setPixel (size_t x, size_t y, const Color color);
const Texture &getTexture ();
}
```
	
Замечание: решено добавить методы RenderTexture в canvas вместо метода getRenderTexture, для того чтобы пользователь не мог изменять ее без нашего ведома.
Замечание: canvasView не стандартизируем, нет необходимости.

1.  Следующие классы имеют только прототипы и определяются и реализуются самостоятельно:
```
incomplete type classes: (для независимости от графической системы)
class Texture;
class VertexArray;
class RenderTarget;
```

Замечание: в связи с этим нельзя непосредственно передавать объекты классов, в качестве параметров функций необходимо использовать ссылки на них.
