# Input

## Create a simple input
```c
if event == InputEventKey && event.pressed:
  if event.keycode == KEY_T:
    print("T was pressed")
```

## Input event types
### Structs
```c
struct InputEvent;               // Get All events.
struct InputEventKey;            // Get All Key events.
struct InputEventWithModifiers;  // e.g. Ctrl + C.
struct InputEventMouse;          // Mouse Icon.
struct InputEventMouseButton;    // Mouse Input.
struct InputMouseMovement;       // Mouse movement x y.
```
### Functions
```c
bool is_action_just_pressed();    //
bool is_action_just_released();   //

bool is_action_pressed();         //
bool is_action_released();        //
```

## Examples

### InputEvent
```c
InputEvent event;
```
### InputEventWithModifiers
```c
InputEventWithModifiers ewm;
```
### InputEventMouse
```c
InputEventMouse em;
```
### InputEventMouseButton
```c
InputEventMouseButton mb;
```
### InputMouseMovement
```c
InputMouseMovement mm;
mm.x;
mm.y;
```


## Input Keys
### Keyboard
```c

```

### Mouse
```c

```

### Controler
```c

```
