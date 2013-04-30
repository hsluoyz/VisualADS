package cnd.conflict.test;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;

import cnd.conflict.graph.Corner;
import cnd.conflict.graph.Rule;

public class ScrollDemo extends JPanel {
    private Rule columnView;
    private Rule rowView;
    private JToggleButton isMetric;
    private ScrollablePicture picture;

    public ScrollDemo() {
        // Start loading the image icon now.
        ImageIcon david = new ImageIcon("images/youngdad.jpeg");

        // Create the row and column headers.
        columnView = new Rule(Rule.HORIZONTAL, true);
        columnView.setPreferredWidth(david.getIconWidth());
        rowView = new Rule(Rule.VERTICAL, true);
        rowView.setPreferredHeight(david.getIconHeight());

        // Create the corners.
        JPanel buttonCorner = new JPanel();
        isMetric = new JToggleButton("cm", true);
        isMetric.setFont(new Font("SansSerif", Font.PLAIN, 11));
        isMetric.setMargin(new Insets(2,2,2,2));
        isMetric.addItemListener(new UnitsListener());
        buttonCorner.add(isMetric); //Use the default FlowLayout

        // Set up the scroll pane.
        picture = new ScrollablePicture(david, columnView.getIncrement());
        JScrollPane pictureScrollPane = new JScrollPane(picture);
        pictureScrollPane.setPreferredSize(new Dimension(300, 250));
        pictureScrollPane.setViewportBorder(
                BorderFactory.createLineBorder(Color.black));

        pictureScrollPane.setColumnHeaderView(columnView);
        pictureScrollPane.setRowHeaderView(rowView);

        pictureScrollPane.setCorner(JScrollPane.UPPER_LEFT_CORNER, 
                                    buttonCorner);
        pictureScrollPane.setCorner(JScrollPane.LOWER_LEFT_CORNER,
                                    new Corner());
        pictureScrollPane.setCorner(JScrollPane.UPPER_RIGHT_CORNER,
                                    new Corner());

        // Put it in this panel.
        setLayout(new BoxLayout(this, BoxLayout.X_AXIS));
        add(pictureScrollPane);
        setBorder(BorderFactory.createEmptyBorder(20,20,20,20));
    }

    class UnitsListener implements ItemListener {
        public void itemStateChanged(ItemEvent e) {
            if (e.getStateChange() == ItemEvent.SELECTED) {
                // Turn it to metric.
                rowView.setIsMetric(true);
                columnView.setIsMetric(true);
            } else {
                // Turn it to inches.
                rowView.setIsMetric(false);
                columnView.setIsMetric(false);
            }
            picture.setMaxUnitIncrement(rowView.getIncrement());
        }
    }

    public static void main(String s[]) {
        JFrame frame = new JFrame("ScrollDemo");
        frame.addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                System.exit(0);
            }
        });
 
        frame.setContentPane(new ScrollDemo());
        frame.pack();
        frame.setVisible(true);
    }
}
