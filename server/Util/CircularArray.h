#ifndef NETWORKEDPHYSICS_CIRCULARARRAY_H
#define NETWORKEDPHYSICS_CIRCULARARRAY_H

template<class T>
class CircularArray {
private:
    int m_size;
    int m_sequence;
    int m_start;
    T *m_values;
public:

    explicit CircularArray(int size) {
        m_size = size;
        m_sequence = 0;
        m_start = 0;
        m_values = new T[m_size];
    }

    ~CircularArray() {
        m_sequence = 0;
        m_start = 0;
        delete[] m_values;
    }

    // Returns the sequence number of the end of the circular array
    int getSequence() const {
        return m_sequence - 1;
    }

    // Returns the value at the given sequence if the sequence is valid, otherwise, returns a nullptr
    T *getValue(int sequence) {
        int start = m_start - 1 < 0 ? 0 : m_start - 1;
        if (sequence < start || sequence > getSequence())
            return nullptr;

        return &m_values[sequence % m_size];
    }

    // Adds a new value to the end of the circular array
    // If the array is full, the oldest item will be overwritten
    void insert(const T &value) {
        m_values[m_sequence % m_size] = value;
        m_sequence++;
        if (m_sequence % m_size == m_start) {
            m_start++;
        }
    }

    // Resets the array
    void clear() {
        m_sequence = 0;
        m_start = 0;
    }
};

#endif //NETWORKEDPHYSICS_CIRCULARARRAY_H
