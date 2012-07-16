// -*- C++ -*-

template <typename T>
void Store<T>::Push(T* elem)
{
    cursor_ = (cursor_ + 1) % elems_.size();
    delete elems_[cursor_];
    elems_[cursor_] = elem;
}

template <typename T>
void Store<T>::Pop()
{
    delete elems_[cursor_];
    elems_[cursor_] = NULL;
    cursor_ = (cursor_ + elems_.size() - 1) % elems_.size();
}

template <typename T>
T* Store<T>::Top() const
{
    return elems_[cursor_];
}


template <typename T>
Store<T>::~Store()
{
    for (size_t i = 0; i < elems_.size(); i++) {
        delete elems_[i];
    }
}
