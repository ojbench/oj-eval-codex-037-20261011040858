"""Compare the executable with a small, independently sorted reference model."""
import random
import subprocess
import sys


def check(seed, executable):
    rng = random.Random(seed)
    records = {}
    commands = []
    expected = []

    def add(name, started=False):
        gender = rng.choice('MF')
        group = rng.randint(1, 20)
        scores = [rng.randint(1, 100) for _ in range(9)]
        commands.append(f'ADD {name} {gender} {group} ' + ' '.join(map(str, scores)))
        if started:
            expected.append('[Error]Cannot add student now.')
        elif name in records:
            expected.append('[Error]Add failed.')
        else:
            records[name] = (gender, group, scores)

    def ranking():
        return sorted(records, key=lambda name: (
            -(sum(records[name][2]) // 9),
            tuple(-x for x in records[name][2]), name))

    for _ in range(rng.randrange(50)):
        add(f's{rng.randrange(30)}')
    # Identical scores exercise name ordering; different totals with the same
    # integer average exercise the distinction between total and average.
    for name, scores in [('tie_b', [50] * 9), ('tie_a', [50] * 9),
                         ('round_a', [51] + [50] * 8),
                         ('round_b', [50] * 8 + [58])]:
        commands.append(f'ADD {name} F 1 ' + ' '.join(map(str, scores)))
        records[name] = ('F', 1, scores)
    commands.append('START')
    order = ranking()
    for _ in range(1000):
        operation = rng.choice(['UPDATE', 'UPDATE', 'QUERY', 'FLUSH', 'PRINTLIST', 'ADD'])
        name = rng.choice(list(records) + ['missing'])
        if operation == 'UPDATE':
            course, score = rng.randrange(9), rng.randrange(101)
            commands.append(f'UPDATE {name} {course} {score}')
            if name in records:
                records[name][2][course] = score
            else:
                expected.append('[Error]Update failed.')
        elif operation == 'QUERY':
            commands.append(f'QUERY {name}')
            expected.append(f'STUDENT {name} NOW AT RANKING {order.index(name) + 1}'
                            if name in records else '[Error]Query failed.')
        elif operation == 'FLUSH':
            commands.append('FLUSH')
            order = ranking()
        elif operation == 'PRINTLIST':
            commands.append('PRINTLIST')
            for rank, student in enumerate(order, 1):
                gender, group, scores = records[student]
                gender = 'male' if gender == 'M' else 'female'
                expected.append(f'{rank} {student} {gender} {group} {sum(scores) // 9}')
        else:
            add(name, started=True)
    commands.append('END')
    result = subprocess.run([executable], input='\n'.join(commands) + '\n',
                            text=True, capture_output=True, check=True)
    assert result.stdout.splitlines() == expected, f'Mismatch for seed {seed}'


executable = sys.argv[1] if len(sys.argv) > 1 else './build/code'
for seed in range(100):
    check(seed, executable)
empty = subprocess.run([executable], input='START\nFLUSH\nPRINTLIST\nQUERY x\nEND\n',
                       text=True, capture_output=True, check=True)
assert empty.stdout == '[Error]Query failed.\n'
print('Passed 100 randomized scenarios and the empty-roster case.')
