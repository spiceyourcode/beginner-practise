def cm_to_m(cm):
    m=cm/100.0
    return m


def m_to_cm(m):
    cm=m*100.0    
    return cm

def find_max(numbers):
    max = numbers[0]
    for number in numbers:
        if number > max:
            max= number      
    return max